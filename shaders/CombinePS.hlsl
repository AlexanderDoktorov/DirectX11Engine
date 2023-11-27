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

float4 main(PS_INPUT input) : SV_TARGET0
{
    // Sample G-buffer data
    float4 position = gBufferPosition.Sample(linearSampler, input.TexCoord);
    float4 normal = gBufferNormal.Sample(linearSampler, input.TexCoord);
    float4 albedo = gBufferAlbedo.Sample(linearSampler, input.TexCoord);

    // Sample accumulated lighting
    float3 lightImpact = lightAccumulation.Sample(linearSampler, input.TexCoord).rgb;

    // Combine G-buffer data and lighting to produce the final color
    float3 finalColor = saturate(lightImpact * albedo.rgb);

    return float4(finalColor, 1.f);
}