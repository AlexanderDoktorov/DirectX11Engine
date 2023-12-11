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

float GetLambertian(const float3 dirToL, const float3 worldNormal)
{
    return max(0.0f, dot(dirToL, worldNormal));
}

float4 CalulateLightImpactForMaterial(float att, float lambertian, float specAngle, MaterialDesc matDesc)
{
    // Ambient
    const float3 ambientLightImpact = matDesc.Ka;
    // Diffuse
    const float3 diffuseLightImpact = matDesc.Kd * (ligthParams.diffuseIntensity * ligthParams.diffuseColor) * att * lambertian;
    // Specular
    const float3 specularLightImpact = matDesc.Ks * ligthParams.diffuseColor * pow(specAngle, matDesc.Ns);
    // Emissive
    const float3 emissiveLightImpact = matDesc.Ke;
    
    return float4((ambientLightImpact + diffuseLightImpact + specularLightImpact + emissiveLightImpact), 1.f);
}

float4 CalulateLightImpact(float att, float lambertian, float specAngle, float shininess)
{
    // Ambient
    const float3 ambientLightImpact = { 0.1f, 0.1f, 0.1f };
    // Diffuse
    const float3 diffuseLightImpact = ligthParams.diffuseIntensity * ligthParams.diffuseColor * att * lambertian;
    // Specular
    const float3 specularLightImpact = ligthParams.diffuseColor * pow(specAngle, shininess);
    
    return float4((ambientLightImpact + diffuseLightImpact + specularLightImpact), 1.f);
}

// Shader that calculates light impact that will be later multiplied by material color
float4 main(in PS_INPUT input) : SV_Target0
{
    float3 worldPosition    = GBufferPosition.Sample(sampleState, input.TexCoord).xyz;
    float3 worldNormal      = GBufferNormal.Sample(sampleState, input.TexCoord).xyz;
    
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
        return CalulateLightImpact(att, lambertian, specAngle, 256);
    }
    else
    {
        MaterialDesc matDesc = RenderMaterials.Load(materialID);
        if (matDesc.hasSpecularMap)
        {
            matDesc.Ns = GBufferSpecular.Sample(sampleState, input.TexCoord).a;
            matDesc.Ks *= GBufferSpecular.Sample(sampleState, input.TexCoord).rgb; // or should we multiply Ks * SpecularColor ?
        }
        return CalulateLightImpactForMaterial(att, lambertian, specAngle, matDesc);
    }
}