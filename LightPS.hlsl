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


float4 ps_main(in PS_INPUT input) : SV_Target0
{
    float4 position = GBufferPosition.Sample(sampleState, input.TexCoord);
    float4 normal = GBufferNormal.Sample(sampleState, input.TexCoord);
    float4 color = GBufferAlbedo.Sample(sampleState, input.TexCoord);

    // Calculate lighting for the pixel and return the value
    
    return color;
}