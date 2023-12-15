Texture2D<float4> gBufferAlbedo : register(t0);
Texture2D<float4> gBufferLight : register(t1);

SamplerState linearSampler : register(s0);

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

float4 main(PS_INPUT input) : SV_TARGET0
{
    return gBufferLight.Sample(linearSampler, input.TexCoord);
}