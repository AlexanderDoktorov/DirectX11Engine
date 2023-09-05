Texture2D<float4> gBufferPosition : register(t0);
Texture2D<float4> gBufferNormal : register(t1);
Texture2D<float4> gBufferAlbedo : register(t2);
Texture2D<float4> lightAccumulation : register(t3);

SamplerState linearSampler : register(s0);

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Sample G-buffer data
    float4 position = gBufferPosition.Sample(linearSampler, input.TexCoord);
    float4 normal = gBufferNormal.Sample(linearSampler, input.TexCoord);
    float4 albedo = gBufferAlbedo.Sample(linearSampler, input.TexCoord);

    // Sample accumulated lighting
    float4 lighting = lightAccumulation.Sample(linearSampler, input.TexCoord);

    // Combine G-buffer data and lighting to produce the final color
    float3 finalColor = albedo.rgb * lighting.rgb;

    return float4(finalColor, 1.0f);
}