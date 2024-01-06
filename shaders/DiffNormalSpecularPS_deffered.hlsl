#include "SlotsLayout.hlsli"
#include "MapsOperations.hlsli"
#include "MaterialBuffer.hlsli"

Texture2D DiffuseMap : register(SLOT_DIFFUSE_MAP);
Texture2D NormalMap : register(SLOT_NORMAL_MAP);
Texture2D SpecularMap : register(SLOT_SPECULAR_MAP);

SamplerState sampleState : register(SLOT_SAMPLER_LINEAR);

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition : POSITION0; // Vertex position in world space (for G-buffer)
    float3 wNormal : NORMAL0;
    float3 wBitangent : BITANGENT0;
    float3 wTangent : TANGENT0;
    float2 textCoord : TEXCOORD; // Texture coordinates
};

struct PSOutput
{
    float4 wPosition : SV_TARGET0;
    float4 wNormal : SV_TARGET1;
    float4 Albedo : SV_TARGET2;
    float4 Specular : SV_TARGET3;
};

static const float specularMapWeigth = 1.f;

PSOutput ps_main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;
    const float4 fragDiffuseSample = DiffuseMap.Sample(sampleState, ps_input.textCoord);
    const float4 fragSpecularSample = SpecularMap.Sample(sampleState, ps_input.textCoord);
    
    /* Diffuse map */
    output.Albedo = float4(ApplyDiffuseMap(matDesc.Kd, fragDiffuseSample.rgb), fragDiffuseSample.a);
    AlphaTest(output.Albedo.a);
    
    /* Normal map */
    const float3x3 tanToWorld = float3x3(
        normalize(ps_input.wTangent),
        normalize(ps_input.wBitangent),
        normalize(ps_input.wNormal)
    );
    output.wNormal = float4(ApplyNormalMap(NormalMap.Sample(sampleState, ps_input.textCoord).xyz, tanToWorld), 0.f);
    
    /* Specular map */
    output.Specular = float4(matDesc.Ks, matDesc.Ns);
    if (matDesc.useSpecColored) {
        output.Specular = ApplySpecularColored(output.Specular, fragSpecularSample, matDesc.hasSpecularAlpha);
    }
    else {
        output.Specular = ApplySpecularRed(output.Specular, fragSpecularSample);
    }
    output.Specular.a /= 1000.f;
    
    output.wPosition = ps_input.wPosition;
    
    return output;
}