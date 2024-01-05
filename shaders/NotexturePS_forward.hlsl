#include "LightInfo.hlsli"
#include "SlotsLayout.hlsli"
#include "MaterialBuffer.hlsli"

StructuredBuffer<LightDesc> lightsBuffer : register(SLOT_LIGHT_STRUCTURED_BUFFER);
SamplerState                sampleState  : register(SLOT_SAMPLER_LINEAR);

cbuffer sceneBuffer : register(SLOT_BUFFER_SCENE)
{
    uint numLights;
    float3 cameraWorldPosition;
};

struct VS_OUT
{
    float4 Position : SV_POSITION;
    float4 wPosition : POSITION0;
    float3 wNormal : NORMAL0;
};

float4 ps_main(VS_OUT ps_input) : SV_TARGET0
{
    const float3 fragWorldNormal    = ps_input.wNormal;
    const float3 fragWorldPosition  = ps_input.wPosition.xyz;
    
    float3 resultiveColor = float3(0.0, 0.0, 0.0);
    for (uint i = 0; i < numLights; ++i)
    {
        LightDesc lightDesc = lightsBuffer[i];
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
        
        /* Specular */
        const float3 spec = Speculate(fragWorldNormal, fragWorldPosition, cameraWorldPosition, li.dirToL, matDesc.Ns); // as we don't have specular map Ns is specular power
        
        // depends on: ambient color of material and light ambient intensity
        const float3 ambient = matDesc.Ka * lightDesc.ambientIntensity;
        // depends on: color of light, color of material, intensity of light, att, and lambertian
        const float3 diffuse = matDesc.Kd * diff * lightDesc.diffuseIntensity * lightDesc.diffuseColor * att;
        // depends on: specular color of material, Kspec, specular intesity of light, and color of light (color of light and specular color of material are blend)
        const float3 specular = matDesc.Ks * diff *spec * lightDesc.specularIntensity * lightDesc.diffuseColor * att;
        
        switch (matDesc.illum)
        {
            case 1:
                resultiveColor += saturate(diffuse); // Color on and Ambient off
                break;
            case 2:
                resultiveColor += saturate(ambient + diffuse); // Color on and Ambient on
                break;
            case 3:
                resultiveColor += saturate(ambient + diffuse + specular); // Highlight on
                break;
        }
    }
    return float4(saturate(resultiveColor), 1.f);
}