// simple function
// Input - 2d coords -> output -> 4d coords

Texture2D CubeTexture : register(t0);
SamplerState CubeSamplerState : register(s0);

struct PS_INPUT
{
    float2 UVs : TEXCOORD0;
};

float4 ps_main(PS_INPUT ps_input) : SV_TARGET
{
    return CubeTexture.Sample(CubeSamplerState, ps_input.UVs);
}
