cbuffer meshDescBuffer : register(b0)
{
    bool   useNormalMap;
    bool   useDiffuseMap;
    bool   useSpecularMap;
    bool   useSpecularMapColored;
    int    materialID;
    float4 albedo;
}

Texture2D DiffuseMap                 : register(t0);
Texture2D NormalMap                  : register(t1);
Texture2D<float> SpecularMap         : register(t2);
Texture2D HeightMap                  : register(t3);
Texture2D<float4> SpecularMapColored : register(t4);

SamplerState Sampler : register(s0);

struct VS_OUT
{
    float4 Position     : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition    : POSITION0; // Vertex position in world space (for G-buffer)
    float3 wNormal      : NORMAL0;
    float3 wBitangent   : BITANGENT0;
    float3 wTangent     : TANGENT0;
    float2 textCoord    : TEXCOORD; // Texture coordinates
};

struct PSOutput
{
    float4 wPosition    : SV_TARGET0;
    float4 wNormal      : SV_TARGET1;
    float4 Albedo       : SV_TARGET2;
    float4 Specular     : SV_TARGET3;
    int    materialID   : SV_TARGET4;
};

PSOutput main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;
    
    // Normal map //
    if (useNormalMap)
    {
        // build the tranform (rotation) into world space
        const float3x3 tanToWorld = float3x3(
            normalize(ps_input.wTangent),
            normalize(ps_input.wBitangent),
            normalize(ps_input.wNormal)
        );
        // unpack the normal from map into tangent space                           --> textID
        const float3 normalSample = NormalMap.Sample(Sampler, ps_input.textCoord).xyz;
        float3 worldNormal = normalSample * 2.0f - 1.0f;
        // bring normal from tanspace into world space
        worldNormal = normalize(mul(worldNormal, tanToWorld));
        output.wNormal = float4(worldNormal, 0.f);
    }
    else
    {
        output.wNormal = float4(ps_input.wNormal, 0.f);
    }
    
    // Diffuse map //
    if(useDiffuseMap)
        output.Albedo = DiffuseMap.Sample(Sampler, ps_input.textCoord);
    else
        output.Albedo = albedo;
    
    // Alpha testing { Pixel discarded if alpha is too small }
    clip(output.Albedo.a < 0.1f ? -1 : 1);
    
    // Specular map //
    if (useSpecularMapColored)
        output.Specular = SpecularMapColored.Sample(Sampler, ps_input.textCoord);
    else if (useSpecularMap)
        output.Specular.a = SpecularMap.Sample(Sampler, ps_input.textCoord);
        
    
    output.wPosition    = ps_input.wPosition;
    output.materialID   = materialID;
    
    return output;
}