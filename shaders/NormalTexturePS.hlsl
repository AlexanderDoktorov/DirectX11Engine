#include "SlotsLayout.hlsli"

cbuffer MaterialDesc : register(SLOT_BUFFER_MATERIAL)
{   
    bool useNormalMap;
    bool useDiffuseMap;
    bool useHeightMap;
    bool useSpecOnlyRed;
    bool useSpecColored;
    bool hasSpecularAlpha;
    int    illum;
    float  Ns;  // shininess
    float3 Kd;  // reflected color diffuse
    float3 Ks;  // reflected color specular
    float3 Ka;  // reflected color ambient
    float3 Ke;  // color emissive 
};

Texture2D DiffuseMap  : register(SLOT_DIFFUSE_MAP);
Texture2D NormalMap   : register(SLOT_NORMAL_MAP);
Texture2D SpecularMap : register(SLOT_SPECULAR_MAP);
Texture2D HeightMap   : register(SLOT_HEIGHT_MAP);

SamplerState sampleState : register(SLOT_SAMPLER_LINEAR);

struct VS_OUT
{
    float4 Position     : SV_POSITION;  // Position in homogeneous clip space
    float4 wPosition    : POSITION0;    // Vertex position in world space (for G-buffer)
    float3 wNormal      : NORMAL0;
    float3 wBitangent   : BITANGENT0;
    float3 wTangent     : TANGENT0;
    float2 textCoord    : TEXCOORD;     // Texture coordinates
};

struct PSOutput
{
    float4 wPosition    : SV_TARGET0;
    float4 wNormal      : SV_TARGET1;
    float4 Albedo       : SV_TARGET2;
    float4 Specular     : SV_TARGET3;
};

static const float specularMapWeigth = 1.f;

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
        // unpack the normal from map into tangent space                 --> textID
        const float3 normalSample = NormalMap.Sample(sampleState, ps_input.textCoord).xyz;
        float3 worldNormal = normalSample * 2.0f - 1.0f;
        // bring normal from tanspace into world space
        worldNormal = normalize(mul(worldNormal, tanToWorld));
        output.wNormal = float4(worldNormal, 0.f);
    }
    else
    {
        output.wNormal = float4(ps_input.wNormal, 0.f);
    }
    
    output.Albedo = float4(Kd, 1.f);
    // Diffuse map //
    if (useDiffuseMap)
    {
        float4 diffuseSample = DiffuseMap.Sample(sampleState, ps_input.textCoord);
        if (any(Kd))
        {
            output.Albedo.rgb = Kd * diffuseSample.rgb;
            output.Albedo.a = diffuseSample.a;
        }
        else
        {
            output.Albedo = diffuseSample;
        }
    }
    
    output.Specular = float4(Ks, Ns);
    if (useSpecOnlyRed)
    {
        output.Specular.a *= SpecularMap.Sample(sampleState, ps_input.textCoord).r;
    }
    else if(useSpecColored)
    {
        if(any(Ks))
            output.Specular.rgb *= SpecularMap.Sample(sampleState, ps_input.textCoord).rgb;
        else
            output.Specular.rgb = SpecularMap.Sample(sampleState, ps_input.textCoord).rgb;
        
        if(hasSpecularAlpha)
            output.Specular.a *= SpecularMap.Sample(sampleState, ps_input.textCoord).a;
    }
    output.Specular.a /= 1000.f;
    
    
    
    // Alpha testing { Pixel discarded if alpha is too small }
    clip(output.Albedo.a < 0.1f ? -1 : 1);
    
    output.wPosition    = ps_input.wPosition;
    
    return output;
}