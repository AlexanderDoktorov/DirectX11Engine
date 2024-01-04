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
    const float4 fragDiffuseColor   = DiffuseMap.Sample(sampleState, ps_input.textCoord);
    const float4 fragSpecularSample = SpecularMap.Sample(sampleState, ps_input.textCoord);
    
    // Alpha test
    AlphaTest(fragDiffuseColor.a);
    
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
        float3 diffuseReflectiveColor = matDesc.Kd;
        if (matDesc.useDiffuseMap)
        {
            if (any(matDesc.Kd))
                diffuseReflectiveColor *= fragDiffuseColor.rgb;
            else
                diffuseReflectiveColor = fragDiffuseColor.rgb;
        }
        /* Specular */
        float3 specularReflectiveColor = matDesc.Ks;
        float specularPower = matDesc.Ns;
        if (matDesc.useSpecColored)
        {
            if (any(matDesc.Ks))
                specularReflectiveColor *= fragSpecularSample.rgb;
            if (matDesc.hasSpecularAlpha)
                specularPower *= fragSpecularSample.a;
        }
        else if (matDesc.useSpecOnlyRed)
        {
            specularPower *= fragSpecularSample.r;
        }
        
        const float3 spec = Speculate(fragWorldNormal, fragWorldPosition, cameraWorldPosition, li.dirToL, specularPower);
        
        // depends on: ambient color of material and light ambient intensity
        const float3 ambient = matDesc.Ka * lightDesc.ambientIntensity;
        // depends on: color of light, color of material, intensity of light, att, and lambertian
        const float3 diffuse = diffuseReflectiveColor * diff * lightDesc.diffuseIntensity * lightDesc.diffuseColor * att;
        // depends on: specular color of material, Kspec, specular intesity of light, and color of light (color of light and specular color of material are blend)
        const float3 specular = specularReflectiveColor * spec * lightDesc.specularIntensity * lightDesc.diffuseColor * att;
        
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
    return float4(saturate(resultiveColor), fragDiffuseColor.a);
}