cbuffer objectCbuff : register(b2)
{
    bool isNormalMapEnabled;
}

Texture2D Texture : register(t3);
Texture2D NormalMap : register(t4);
Texture2D SpecularMap : register(t5);
SamplerState Sampler : register(s0);

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition : POSITION0; // Vertex position in world space (for G-buffer)
    float4 wNormal : NORMAL0;
    float2 textCoord : TEXCOORD0; // Texture coordinates
    uint   materialID : MATERIALID0;
    float3x3 TBN : TBN0;
};

struct PSOutput
{
    float4 wPosition    : SV_TARGET0;
    float4 wNormal      : SV_TARGET1;
    float4 Albedo       : SV_TARGET2;
    uint   materialID   : SV_TARGET3;
};

PSOutput main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;
    
    if (isNormalMapEnabled)
    {
        output.wNormal = NormalMap.Sample(Sampler, ps_input.textCoord);
        output.wNormal.xyz = output.wNormal.xyz * 2.f - 1.f; // ranges from -1.f to +1.f now
        output.wNormal = normalize(float4(mul(output.wNormal.xyz, ps_input.TBN), 1.f));
    }
    else
    {
        output.wNormal = ps_input.wNormal;
    }

    output.Albedo = Texture.Sample(Sampler, ps_input.textCoord);
    output.wPosition = ps_input.wPosition;
    output.materialID = ps_input.materialID;
    
    return output;
}