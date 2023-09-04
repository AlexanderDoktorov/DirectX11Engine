// Transforms coordinates

// PVW if transposed (W*V*P)^T = P^T * V^T * W^T (DX - row major / HLSL - column major)
cbuffer transform : register(b0)
{
    matrix WorldToViewToProjection;
    matrix ModelToView;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 textcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUT
{
    float2 textcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 sv_pos : SV_POSITION;
};

VS_OUT vs_main(VS_INPUT input)
{
    VS_OUT vs_output = (VS_OUT) 0;
    
    vs_output.textcoord = input.textcoord;
    vs_output.normal = mul(input.normal, (float3x3) ModelToView);
    vs_output.sv_pos = mul(float4(input.position, 1.f), WorldToViewToProjection);
    //vs_output.world_pos = (float3) mul(float4(input.position, 1.0f), ModelToView);
    
    return vs_output;
}