cbuffer transform : register(b1)
{
    matrix World;
    matrix View;
    matrix Projection;
};

cbuffer objectCbuff : register(b2)
{
    bool isNormalMapEnabled;
}

Texture2D Texture : register(t3);
Texture2D NormalMap : register(t4);
SamplerState Sampler : register(s0);

struct VS_OUT
{
    float4 Position     : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition    : POSITION0; // Vertex position in world space (for G-buffer)
    float4 wNormal      : NORMAL0;
    float2 textCoord    : TEXCOORD0; // Texture coordinates
    float3x3 TBN        : TBN0;
};

struct PSOutput
{
    float4 wPosition : SV_TARGET0;
    float4 wNormal : SV_TARGET1;
    float4 Albedo : SV_TARGET2;
};

PSOutput main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;
    
    if (isNormalMapEnabled)
    {
        output.wNormal = NormalMap.Sample(Sampler, ps_input.textCoord);
        output.wNormal.x = output.wNormal.x * 2.f - 1.f; // ranges from -1.f to +1.f now
        output.wNormal.y = output.wNormal.y * 2.f - 1.f;
        output.wNormal.z = -output.wNormal.z;
        output.wNormal = float4(mul(output.wNormal.xyz, ps_input.TBN), 1.f);
    }
    else
    {
        output.wNormal = -ps_input.wNormal;
    }

    output.Albedo   = Texture.Sample(Sampler, ps_input.textCoord);
    output.wPosition = ps_input.wPosition;
    
    return output;
}