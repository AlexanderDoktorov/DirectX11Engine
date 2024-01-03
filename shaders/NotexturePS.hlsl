#include "DefaultValues.hlsli"

cbuffer MaterialDesc : register(b0)
{
    bool useNormalMap;
    bool useDiffuseMap;
    bool useHeightMap;
    bool useSpecOnlyRed;
    bool useSpecColored;
    bool hasSpecularAlpha;
    int illum;
    float Ns; // shininess
    float3 Kd; // reflected color diffuse
    float3 Ks; // reflected color specular
    float3 Ka; // reflected color ambient
    float3 Ke; // color emissive 
};


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

PSOutput main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;

    output.wPosition    = ps_input.wPosition;
    output.wNormal      = float4(ps_input.wNormal, 0.f);
    output.Albedo       = float4(Kd, 1.f);
    output.Specular     = float4(Ks, Ns / 1000.f);
    
    return output;
}