Texture2D Texture : register(t3);
Texture2D NormalMap : register(t4);
SamplerState Sampler : register(s0);

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float3 WorldPosition : WORLDPOS0; // Vertex position in world space (for G-buffer)
    float2 TexCoord : TEXCOORD0; // Texture coordinates
};

struct PSOutput
{
    float4 Position : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 Albedo : SV_TARGET2;
};


PSOutput main(VS_OUT ps_input) : SV_TARGET
{
    PSOutput output = (PSOutput) 0;

    output.Albedo   = Texture.Sample(Sampler, ps_input.TexCoord);
    output.Normal   = NormalMap.Sample(Sampler, ps_input.TexCoord);
    output.Position = float4(ps_input.WorldPosition, 0.f);
    
    return output;
}