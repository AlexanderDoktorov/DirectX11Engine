#include "LightInfo.hlsli"

// Texture resources containing G-buffer data
Texture2D<float4> GBufferPosition   : register(t0);
Texture2D<float4> GBufferNormal     : register(t1);
Texture2D<float4> GBufferAlbedo     : register(t2);
Texture2D<float4> GBufferSpecular   : register(t3);
Texture2D<int>    GBufferMatID      : register(t4);
SamplerState      sampleState       : register(s0);

struct PointLightDesc
{
    float3 worldPosition;
    float3 ambientColor;
    float3 diffuseColor;
    float3 specularColor;
    float  ambientIntensity;
    float  diffuseIntensity;
    float  specularIntensity;
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
    PointLightDesc lightParams;
};

struct MaterialDesc
{
    bool hasNormalMap;
    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasSpecularMapColored;
    bool hasHeightMap;
    float3 Kd; // reflected color diffuse
    float3 Ks; // reflected color specular
    float3 Ka; // reflected color ambient
    float3 Ke; //           color emmisive
    float  Ns; // shininess
    int    illum;
};

StructuredBuffer<MaterialDesc> RenderMaterials : register(t5);

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

// Shader that calculates light impact that will be later multiplied by material color
float4 main(in PS_INPUT input) : SV_Target0
{
    float3 fragWorldPos     = GBufferPosition.Sample(sampleState, input.TexCoord).xyz;
    float3 fragWorldNormal  = GBufferNormal.Sample(sampleState, input.TexCoord).xyz;
    float4 fragDiffuseColor = GBufferAlbedo.Sample(sampleState, input.TexCoord);
    float3 fragSpecColor    = GBufferSpecular.Sample(sampleState, input.TexCoord).rgb;
    float  fragShininess    = GBufferSpecular.Sample(sampleState, input.TexCoord).a;
    
    LightInfo li = BuildLightInfo(lightParams.worldPosition, fragWorldPos);
    
    float att  = Attenuate(lightParams.Catt, lightParams.Latt, lightParams.Qatt, li.distToL);
    float diff = Diffusate(fragWorldNormal, li.dirToL);
    
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
        const float3 ambientColor = { 0.05f, 0.05f, 0.05f };
        const float  shininess = 128;
        const float  spec = Speculate(fragWorldNormal, fragWorldPos, worldCameraPosition, li.dirToL, shininess);
        
        return float4((ambientColor + lightParams.diffuseColor * lightParams.diffuseIntensity * diff * att) * fragDiffuseColor.rgb + lightParams.diffuseColor * spec * att, fragDiffuseColor.a);
    }
    else
    {
        MaterialDesc matDesc = RenderMaterials.Load(materialID);
        
        float3 ambientReflectiveColor = matDesc.Ka; // ambiemnt color of material if ambeint map not present
        float3 diffuseReflectiveColor = matDesc.Kd; // diffuse color of material if diffuse map not present
        float3 specularReflectiveColor = matDesc.Ks; // specular coor of material if specular color map not present
        float  shininess = matDesc.Ns; // material shininess if R8 spec map not present
       
        if (matDesc.hasSpecularMapColored)
        {
            shininess = fragShininess;
            specularReflectiveColor = fragSpecColor;
        }
        else if (matDesc.hasSpecularMap)
        {
            shininess = fragShininess;
        }
        
        if (matDesc.hasDiffuseMap)
            diffuseReflectiveColor = fragDiffuseColor.rgb;
        
        const float spec = Speculate(fragWorldNormal, fragWorldPos, worldCameraPosition, li.dirToL, shininess);
        
        // depends on: ambient color of material and light ambient intensity
        const float3 ambient = ambientReflectiveColor * lightParams.ambientIntensity;
        // depends on: color of light, color of material, intensity of light, att, and lambertian
        const float3 diffuse = diffuseReflectiveColor * diff * lightParams.diffuseIntensity * lightParams.diffuseColor * att;
        // depends on: specular color of material, Kspec, specular intesity of light, and color of light (color of light and specular color of material are blend)
        const float3 specular = specularReflectiveColor * spec * lightParams.specularIntensity * lightParams.diffuseColor * att;
        
        // switch illumination model
        switch (matDesc.illum)
        {
            case 1:
                return float4(diffuse, fragDiffuseColor.a); // Color on and Ambient off
            case 2:
                return float4(ambient + diffuse, fragDiffuseColor.a); // Color on and Ambient on
            case 3:
                return float4(ambient + diffuse + specular, fragDiffuseColor.a); // Highlight on
        }
        
        return float4(1.0, 0.f, 0.f, 1.f); // red color as a sign that material was not used (or illum model isn't [0:2])
    }
}