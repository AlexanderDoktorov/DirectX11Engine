cbuffer transform : register(b0)
{
    matrix WorldToViewToProjection;
    matrix ModelToView;
};

struct VS_OUT
{
    float4 sv_pos : SV_POSITION;
    float4 normal : NORMAL0;
};

VS_OUT main(float3 pos : POSITION, float3 normal : NORMAL0)
{
    VS_OUT vs_out = (VS_OUT) 0;
    
    vs_out.sv_pos = mul(float4(pos, 1.f), WorldToViewToProjection);
    vs_out.normal = mul(float4(normal, 1.f), (float4x4) ModelToView); // CAREFUL
    
    return vs_out;
}
