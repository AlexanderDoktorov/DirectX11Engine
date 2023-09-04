// simple function
// Input - 2d coords -> output -> 4d coords

Texture2D CubeTexture : register(t3);
SamplerState CubeSamplerState : register(s0);

struct VSOutput
{
    float2 textcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 sv_pos : SV_POSITION;
};

struct PSOutput
{
    float4 Position : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 Color : SV_TARGET2;
};

Texture2D<float4> gBufferPos : register(t0);
Texture2D<float4> gBufferNorm : register(t1);
Texture2D<float4> gBufferAlbedo : register(t2);

PSOutput ps_main(VSOutput ps_input) : SV_TARGET
{
    PSOutput output = (PSOutput) 0;
    
    output.Color = CubeTexture.Sample(CubeSamplerState, ps_input.textcoord);
    output.Normal = float4(ps_input.normal, 1.f);
    output.Position = ps_input.sv_pos;
    
    return output;
}