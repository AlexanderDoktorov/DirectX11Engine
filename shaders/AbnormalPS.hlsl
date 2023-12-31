cbuffer meshDescBuffer : register(b0)
{
    bool useNormalMap;
    bool useDiffuseMap;
    bool useSpecularMap;
    int materialID;
    float3 albedo;
}

Texture2DArray DiffuseMaps : register(t0);
Texture2DArray SpecularMaps : register(t2);
Texture2DArray HeightMaps : register(t3);

SamplerState Sampler : register(s0);

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition : POSITION0; // Vertex position in world space (for G-buffer)
    float3 wNormal : NORMAL0;
    float2 textCoord : TEXCOORD; // Texture coordinates
};

struct PSOutput
{
    float4 wPosition : SV_TARGET0;
    float4 wNormal : SV_TARGET1;
    float4 Albedo : SV_TARGET2;
    float4 Specular : SV_TARGET3;
    int materialID : SV_TARGET4;
};

PSOutput main(VS_OUT ps_input)
{
    PSOutput output = (PSOutput) 0;
    
    output.wNormal = float4(ps_input.wNormal, 0.f);
    
    // Diffuse map //
    if (useDiffuseMap)
        output.Albedo = DiffuseMaps.Sample(Sampler, float3(ps_input.textCoord, 0));
    else
        output.Albedo = float4(albedo, 0.f);
    
    // Specular map //
    if (useSpecularMap)
        output.Specular = SpecularMaps.Sample(Sampler, float3(ps_input.textCoord, 0));
    
    output.wPosition = ps_input.wPosition;
    output.materialID = materialID;
    
    return output;
}