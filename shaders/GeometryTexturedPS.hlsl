// simple function
// Input - 2d coords -> output -> 4d coords

Texture2D Texture : register(t3);
SamplerState Sampler : register(s0);

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float3 WorldPosition : TEXCOORD0; // Vertex position in world space (for G-buffer)
    float3 WorldNormal : TEXCOORD1; // Vertex normal in world space (for G-buffer)
    float2 TexCoord : TEXCOORD2; // Texture coordinates
};

struct PSOutput
{
    float4 wPosition : SV_TARGET0;
    float4 wNormal : SV_TARGET1;
    float4 Albedo : SV_TARGET2;
    float4 Specular : SV_TARGET3;
    int materialID : SV_TARGET4;
};

PSOutput main(VS_OUT ps_input) : SV_TARGET
{
    PSOutput output = (PSOutput) 0;
    
    output.Albedo = float4(Texture.Sample(Sampler, ps_input.TexCoord).rgb, 1.f);
    output.wNormal = float4(ps_input.WorldNormal, 0.f);
    output.wPosition = float4(ps_input.WorldPosition, 0.f);
    output.materialID = -1;

    return output;
}