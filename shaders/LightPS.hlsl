// Texture resources containing G-buffer data
Texture2D<float4> GBufferPosition : register(t0);
Texture2D<float4> GBufferNormal : register(t1);
Texture2D<float4> GBufferAlbedo : register(t2);
SamplerState sampleState : register(s0);

struct LightDesc
{
    float3 pos;
    float3 diffuseColor;
    float diffuseIntensity;
    float Catt;
    float Latt;
    float Qatt;
};

// Constant buffer for lighting parameters
cbuffer LightingPassPSConstants : register(b1)
{
    LightDesc ligthParams;
};

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

static float3 ambient = float3(0.1, 0.1, 0.1);

float4 ApplyLightImpact(float3 normal, float3 world_pos, float4 materialcolor)
{
    const float3 vToL = ligthParams.pos - world_pos;
    const float3 distToL = length(vToL);
    const float3 dirToL = normalize(vToL);
    const float att = 1.0f / (ligthParams.Catt + ligthParams.Latt * distToL + ligthParams.Qatt * (distToL * distToL));
    const float3 diffuse = ligthParams.diffuseColor * ligthParams.diffuseIntensity * att * max(0.0f, dot(dirToL, normal));

    return float4(saturate((diffuse + ambient) * materialcolor.rgb), 1.0f);
}


float4 main(in PS_INPUT input) : SV_Target0
{
    float3 position = GBufferPosition.Sample(sampleState, input.TexCoord).xyz;
    float3 normal = GBufferNormal.Sample(sampleState, input.TexCoord).xyz;
    float4 color = GBufferAlbedo.Sample(sampleState, input.TexCoord);
    
    return ApplyLightImpact(normal, position, color);
}