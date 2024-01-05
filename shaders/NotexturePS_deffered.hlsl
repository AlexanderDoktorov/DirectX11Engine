#include "MaterialBuffer.hlsli"

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition : POSITION0; // Vertex position in world space (for G-buffer)
    float3 wNormal : NORMAL0;
};

struct PSOutput
{
    float4 wPosition : SV_TARGET0;
    float4 wNormal : SV_TARGET1;
    float4 Albedo : SV_TARGET2;
    float4 Specular : SV_TARGET3;
};

PSOutput ps_main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;

    output.wPosition    = ps_input.wPosition;
    output.wNormal      = float4(ps_input.wNormal, 0.f);
    output.Albedo       = float4(matDesc.Kd, 1.f);
    output.Specular     = float4(matDesc.Ks, matDesc.Ns / 1000.f);
    
    return output;
}