#include "LightInfo.hlsli"

// Texture resources containing G-buffer data
Texture2D<float4> GBufferPosition   : register(t0);
Texture2D<float4> GBufferNormal     : register(t1);
Texture2D<float4> GBufferAlbedo     : register(t2);
Texture2D<float4> GBufferSpecular   : register(t3);
SamplerState      sampleState       : register(s0);

cbuffer CameraBuffer : register(b0)
{
    float3 worldCameraPosition;
    float  padding[1];
};

// Constant buffer for lighting parameters
cbuffer LightingBuffer : register(b1)
{
    LightDesc lightParams;
};

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

static const float3 ambientReflectiveColor = float3(0.05f, 0.05f, 0.05f);

// Shader that calculates light impact that will be later multiplied by material color
float4 main(in PS_INPUT input) : SV_Target0
{
    float3 fragWorldPos     = GBufferPosition.Sample(sampleState, input.TexCoord).xyz;
    float3 fragWorldNormal  = GBufferNormal.Sample(sampleState, input.TexCoord).xyz;
    float4 fragDiffuseColor = GBufferAlbedo.Sample(sampleState, input.TexCoord);
    float4 fragSpecSample   = GBufferSpecular.Sample(sampleState, input.TexCoord);
    
    LightInfo li = BuildLightInfo(lightParams.worldPosition, fragWorldPos);
    
    float att  = Attenuate(lightParams.Catt, lightParams.Latt, lightParams.Qatt, li.distToL);
    
    // Calucalte diffuse coefficient
    float diff = 0.f;
    switch (lightParams.typeId)
    {
        case LIGHT_TYPE_POINT_LIGHT:
            diff = DiffusePointLight(fragWorldNormal, li.dirToL);
            break;
        case LIGHT_TYPE_SPOTLIGHT:
            diff = DiffuseSpotlight(lightParams.spotlightDirection, li.dirToL, lightParams.spotlightAngle);
            break;
    }
    
    const float3 diffuseReflectiveColor  = fragDiffuseColor.rgb;
    const float3 specularReflectiveColor = fragSpecSample.rgb;
    const double specularPower           = fragSpecSample.a * 1000.f;
    
    const float spec = Speculate(fragWorldNormal, fragWorldPos, worldCameraPosition, li.dirToL, specularPower);
    
    // depends on: ambient color of material and light ambient intensity
    const float3 ambient = ambientReflectiveColor * lightParams.ambientIntensity;
    // depends on: color of light, color of material, intensity of light, att, and lambertian
    const float3 diffuse = diffuseReflectiveColor * diff * lightParams.diffuseIntensity * lightParams.diffuseColor * att;
    // depends on: specular color of material, Kspec, specular intesity of light, and color of light (color of light and specular color of material are blend)
    const float3 specular = specularReflectiveColor * spec * lightParams.specularIntensity * lightParams.diffuseColor * att;
    
    return float4(saturate(ambient + diffuse + specular), fragDiffuseColor.a); // Highlight on
}