#include "DefaultValues.hlsli"

cbuffer MeshBuffer : register(b0)
{
    float4 albedo;
    int matID;              
    bool useNormalMap;      /* unused */
    bool useDiffuseMap;     /* unused */
    bool useHeightMap;      /* unused */
    bool useSpecularMap;    /* unused */
    bool useSpecularAlpha;  /* unused */
}

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
    int materialID : SV_TARGET4;
};

PSOutput main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;

    output.wPosition    = ps_input.wPosition;
    output.wNormal      = float4(ps_input.wNormal, 0.f);
    output.Albedo       = albedo;
    output.Specular     = defaultSpecularColor;
    output.materialID   = matID;
    
    return output;
}