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
    float3 wNormal      : NORMAL0;
    float3 wBitangent   : BITANGENT0;
    float3 wTangent     : TANGENT0;
    float2 textCoord    : TEXCOORD; // Texture coordinates
    uint   materialID   : MATERIALID0;
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
        // build the tranform (rotation) into tangent space
            const float3x3 tanToWorld = float3x3(
            normalize(ps_input.wTangent),
            normalize(ps_input.wBitangent),
            normalize(ps_input.wNormal)
        );
        // unpack the normal from map into tangent space        
            const float3 normalSample = NormalMap.Sample(Sampler, ps_input.textCoord).xyz;
            float3 n = normalSample * 2.0f - 1.0f;
            n.y = -n.y;
        // bring normal from tanspace into view space
            output.wNormal = float4(mul(n, tanToWorld), 0.f);
    }
    else
    {
        output.wNormal = float4(ps_input.wNormal, 0.f);;
    }

    output.Albedo       = Texture.Sample(Sampler, ps_input.textCoord);
    output.wPosition    = ps_input.wPosition;
    output.materialID   = ps_input.materialID;
    
    return output;
}