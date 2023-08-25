cbuffer transform : register(b0)
{
    matrix WorldToViewToProjection;
    matrix ModelToView;
};

struct VS_OUT
{
    float3 normal : NORMAL0;
    float3 world_pos : WORLDPOS;
    float4 sv_pos : SV_POSITION;
};

VS_OUT main(float3 pos : POSITION, float3 normal : NORMAL0)
{
    VS_OUT vs_out = (VS_OUT) 0;
    
    vs_out.sv_pos = mul(float4(pos, 1.f), WorldToViewToProjection);
    vs_out.normal = mul(normal, (float3x3) ModelToView);
    vs_out.world_pos = (float3) mul(float4(pos, 1.0f), ModelToView);
    
    return vs_out;
}
