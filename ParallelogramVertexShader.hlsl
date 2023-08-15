// Transforms coordinates

cbuffer Cbuf : register(b0)
{
    matrix WVPmatrix; // PVW if transposed (W*V*P)^T = P^T * V^T * W^T (DX - row major / HLSL - column major)
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 textcoord : TEXCOORD0;
};

struct VS_OUT
{
    float2 textcoord : TEXCOORD0;
    float4 pos : SV_Position;
};

VS_OUT vs_main(VS_INPUT input)
{
    VS_OUT vs_output = (VS_OUT) 0;
    
    vs_output.textcoord = input.textcoord;
    vs_output.pos = mul(float4(input.position, 1.f), WVPmatrix);
    
    return vs_output;
}