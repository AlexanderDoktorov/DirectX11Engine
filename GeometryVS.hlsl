cbuffer transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_OUT
{
    float4 PixelScreenPos : SV_POSITION;
    float4 WorldPos : TEXCOORD0;
    float4 Normal : TEXCOORD1;
};

VS_OUT main(float3 pos : POSITION, float3 normal : NORMAL0)
{
    VS_OUT vs_out = (VS_OUT) 0;
    matrix WorldViewProjection = mul(mul(World, View), Projection);
    
    vs_out.PixelScreenPos = mul(float4(pos, 1.f), WorldViewProjection);
    vs_out.WorldPos = mul(float4(pos, 1.f), World);
    vs_out.Normal = normalize(mul(float4(normal, 0.0f), World));
    
    return vs_out;
}
