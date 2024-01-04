#include "SlotsLayout.hlsli"

struct MaterialDesc
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

cbuffer materialBuffer : register(SLOT_BUFFER_MATERIAL)
{
    MaterialDesc matDesc;
};