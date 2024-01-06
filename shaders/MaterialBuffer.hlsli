#include "SlotsLayout.hlsli"

struct MaterialDesc
{
    float3 Kd;
    bool useSpecColored;
    float3 Ks;
    bool hasSpecularAlpha;
    float3 Ka;
    float Ns;
    float3 Ke;
    int illum;
};

cbuffer materialBuffer : register(SLOT_BUFFER_MATERIAL)
{
    MaterialDesc matDesc;
};