#include "MapsOperations.hlsli"
#include "LightInfo.hlsli"
#include "SlotsLayout.hlsli"
#include "MaterialBuffer.hlsli"

Texture2D DiffuseMap    : register(SLOT_DIFFUSE_MAP);
Texture2D NormalMap     : register(SLOT_NORMAL_MAP);
Texture2D SpecularMap   : register(SLOT_SPECULAR_MAP);
StructuredBuffer<LightDesc> lightsBuffer : register(SLOT_LIGHT_STRUCTURED_BUFFER);
SamplerState sampleState : register(SLOT_SAMPLER_LINEAR);

cbuffer sceneBuffer : register(SLOT_BUFFER_SCENE)
{
    uint numLights;
    float3 cameraWorldPosition;
};

cbuffer lightBuffer : register(SLOT_BUFFER_LIGHT)
{
    LightDesc lightParams;
};

struct VS_OUT
{
    float4 Position : SV_POSITION;
    float4 wPosition : POSITION0;
    float3 wNormal : NORMAL0;
    float3 wBitangent : BITANGENT0;
    float3 wTangent : TANGENT0;
    float2 textCoord : TEXCOORD;
};

float4 ps_main(VS_OUT ps_input) : SV_TARGET0
{
    /* Normal map */
    const float3x3 tanToWorld = float3x3(
        normalize(ps_input.wTangent),
        normalize(ps_input.wBitangent),
        normalize(ps_input.wNormal)
    );
    const float3 fragWorldNormal    = ApplyNormalMap(NormalMap.Sample(sampleState, ps_input.textCoord).xyz, tanToWorld);
    const float3 fragWorldPosition  = ps_input.wPosition.xyz;
    const float4 fragDiffuseSample  = DiffuseMap.Sample(sampleState, ps_input.textCoord);
    const float4 fragSpecularSample = SpecularMap.Sample(sampleState, ps_input.textCoord);
    
    // Alpha test
    AlphaTest(fragDiffuseSample.a);
    
    float3 resultiveColor = float3(0.0, 0.0, 0.0);
    for (uint i = 0; i < numLights; ++i)
    {
        /* HARDCODED */ LightDesc lightDesc = lightParams; /* HARDCODED */
        LightInfo li = BuildLightInfo(lightDesc.worldPosition, fragWorldPosition);
        const float3 att = Attenuate(lightDesc.Catt, lightDesc.Latt, lightDesc.Qatt, li.distToL);
        
        /* Diffuse */
        float diff = 0.f;
        switch (lightDesc.typeId)
        {
            case LIGHT_TYPE_POINT_LIGHT:
                diff = DiffusePointLight(fragWorldNormal, li.dirToL);
                break;
            case LIGHT_TYPE_SPOTLIGHT:
                diff = DiffuseSpotlight(lightDesc.spotlightDirection, li.dirToL, lightDesc.spotlightAngle);
                break;
            case LIGHT_TYPE_DIRECTIONAL:
                //ApplyDirectionalLight();
                break;
        }
        float3 diffuseReflectiveColor = ApplyDiffuseMap(matDesc.Kd, fragDiffuseSample.rgb);
        /* Specular */
        float4 specularMapped = float4(matDesc.Ks, matDesc.Ns);
        if(matDesc.useSpecColored) {
            specularMapped = ApplySpecularColored( specularMapped, fragSpecularSample, matDesc.hasSpecularAlpha );
        }
        else {
            specularMapped = ApplySpecularRed(specularMapped, fragSpecularSample);
        }
        
        const float3 spec = Speculate(fragWorldNormal, fragWorldPosition, cameraWorldPosition, li.dirToL, specularMapped.a);
        
        // depends on: ambient color of material and light ambient intensity
        const float3 ambient = ambient_default * lightDesc.ambientIntensity;
        // depends on: color of light, color of material, intensity of light, att, and lambertian
        const float3 diffuse = diffuseReflectiveColor * diff * lightDesc.diffuseIntensity * lightDesc.diffuseColor * att;
        // depends on: specular color of material, Kspec, specular intesity of light, and color of light (color of light and specular color of material are blend)
        const float3 specular = specularMapped.rgb * diff * spec * lightDesc.specularIntensity * lightDesc.diffuseColor * att;
        
        resultiveColor += ApplyIlluminationModel(matDesc.illum, ambient, diffuse, specular);
    }
    return float4(saturate(resultiveColor), fragDiffuseSample.a);
}