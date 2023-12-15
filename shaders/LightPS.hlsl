// Texture resources containing G-buffer data
Texture2D<float4> GBufferPosition   : register(t0);
Texture2D<float4> GBufferNormal     : register(t1);
Texture2D<float4> GBufferAlbedo     : register(t2);
Texture2D<float4> GBufferSpecular   : register(t3);
Texture2D<int>    GBufferMatID      : register(t4);
SamplerState      sampleState       : register(s0);

struct PointLightDesc
{
    float3 pos;
    float3 diffuseColor;
    float  diffuseIntensity;
    float  Catt;
    float  Latt;
    float  Qatt;
};

cbuffer CameraBuffer : register(b0)
{
    float3 worldCameraPosition;
    float  padding[1];
};

// Constant buffer for lighting parameters
cbuffer LightingPassPSConstants : register(b1)
{
    PointLightDesc ligthParams;
};

struct MaterialDesc
{
    bool hasNormalMap;
    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasHeightMap;
    float3 Kd; // reflected color diffuse
    float3 Ks; // reflected color specular
    float3 Ka; // reflected color ambient
    float3 Ke; //           color emmisive
    float  Ns; // shininess
};

StructuredBuffer<MaterialDesc> RenderMaterials : register(t5);

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

float4 ApplyLightForMaterial(float4 albedo, float3 dirToL, float3 worldNormal, float att, float specAngle, MaterialDesc matDesc)
{
    // Ambient
    const float3 ambientColor = matDesc.Ka;
    // Diffuse
    const float3 diffuseIntensity = max(0.0f, dot(dirToL, worldNormal)) * ligthParams.diffuseIntensity;
    const float3 diffuseColor = matDesc.Kd * ligthParams.diffuseColor * att * diffuseIntensity;
    // Specular
    const float3 specularIntensity = pow(specAngle, matDesc.Ns) * att * diffuseIntensity;
    const float3 specularColor = matDesc.Ks * specularIntensity;
    // Emissive
    const float3 emissiveColor = matDesc.Ke;
    
    // Combine diffuse and specular
    float3 lightingImpact = diffuseColor + specularColor;
    
    // Final color
    float3 finalColor = (ambientColor + emissiveColor + diffuseColor) * albedo.rgb + specularColor;
    
    return float4(finalColor, albedo.a);
}

float4 ApplyLight(float4 albedo, float3 dirToL, float3 worldNormal, float att, float specAngle, float shininess)
{
     // Ambient
    const float3 ambientColor = { 0.05f, 0.05f, 0.05f };
    // Diffuse
    const float3 diffuseIntensity = max(0.0f, dot(dirToL, worldNormal)) * ligthParams.diffuseIntensity * att;
    const float3 diffuseColor = ligthParams.diffuseColor * diffuseIntensity;
    // Specular
    const float3 specularIntensity = pow(specAngle, shininess) * att * diffuseIntensity;
    const float3 specularColor = ligthParams.diffuseColor * specularIntensity;
    
    // Combine diffuse and specular
    float3 lightingImpact = diffuseColor + specularColor;
    
    // Final color
    float3 finalColor = (ambientColor + diffuseColor) * albedo.rgb + specularColor;
    
    return float4(finalColor, albedo.a);
}

// Shader that calculates light impact that will be later multiplied by material color
float4 main(in PS_INPUT input) : SV_Target0
{
    float3 worldPosition    = GBufferPosition.Sample(sampleState, input.TexCoord).xyz;
    float3 worldNormal      = GBufferNormal.Sample(sampleState, input.TexCoord).xyz;
    float4 albedo           = GBufferAlbedo.Sample(sampleState, input.TexCoord);
    
    
    // Directions
    const float3    vToL    = ligthParams.pos - worldPosition;
    const float     distToL = length(vToL);
    const float3    dirToL  = normalize(vToL);
    const float     att     = 1.0f / (ligthParams.Catt + ligthParams.Latt * distToL + ligthParams.Qatt * (distToL * distToL));
    // Diffuse component
    const float     lambertian  = max(dot(worldNormal, dirToL), 0.0);
    // Specular component
    const float3 viewDir    = normalize(worldCameraPosition - worldPosition);
    const float3 reflectDir = reflect(-dirToL, worldNormal);
    const float  specAngle  = saturate(dot(viewDir, reflectDir));
    // Get material id from texture
    uint textureWidth;
    uint textureHeight;
    GBufferMatID.GetDimensions(textureWidth, textureHeight);
    int2 texelCoordinates;
    texelCoordinates.x = input.TexCoord.x * textureWidth;
    texelCoordinates.y = input.TexCoord.y * textureHeight;
    int materialID = GBufferMatID.Load(int3(texelCoordinates, 0));
    
    if (materialID < 0)
    {
        return ApplyLight(albedo, dirToL, worldNormal, att, specAngle, 256);
    }
    else
    {
        MaterialDesc matDesc = RenderMaterials.Load(materialID);
        if (matDesc.hasSpecularMap)
        {
            matDesc.Ns = GBufferSpecular.Sample(sampleState, input.TexCoord).a;
            matDesc.Ks *= GBufferSpecular.Sample(sampleState, input.TexCoord).rgb; // or should we multiply Ks * SpecularColor ?
        }
        return ApplyLightForMaterial(albedo, dirToL, worldNormal, att, specAngle, matDesc);
    }
}