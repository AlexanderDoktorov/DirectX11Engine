#include "MapsOperations.hlsli"
#include "SlotsLayout.hlsli"

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition : POSITION0; // Vertex position in world space (for G-buffer)
    float3 wNormal : NORMAL0;
    float2 textCoord : TEXCOORD; // Texture coordinates
};

Texture2D DiffuseMap : register(SLOT_DIFFUSE_MAP);
Texture2D SpecularMap : register(SLOT_SPECULAR_MAP);

SamplerState sampleState : register(s0);

cbuffer ObjectCBuf
{
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
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
    
    output.wNormal = float4(ps_input.wNormal, 0.f);
    
    /* Diffuse map */
    output.Albedo = DiffuseMap.Sample(sampleState, ps_input.textCoord);
    
    // Alpha testing { Pixel discarded if alpha is too small }
    AlphaTest(output.Albedo.a);
    
    output.wPosition = ps_input.wPosition;
    output.Specular  = SpecularMap.Sample(sampleState, ps_input.textCoord);
    
    return output;
}