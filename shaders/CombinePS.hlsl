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
    // Sample G-buffer data
    float3 albedo = gBufferAlbedo.Sample(linearSampler, input.TexCoord).rgb;

    // Sample accumulated lighting
    float3 lightImpact = gBufferLight.Sample(linearSampler, input.TexCoord).rgb;

    // Combine G-buffer data and lighting to produce the final color
    float3 finalColor = saturate(lightImpact * albedo);

    return float4(finalColor, 1.f);
}