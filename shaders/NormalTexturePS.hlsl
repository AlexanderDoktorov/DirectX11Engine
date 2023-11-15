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
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float4 WorldPosition : TEXCOORD0; // Vertex position in world space (for G-buffer)
    float4 WorldNormal : TEXCOORD1; // Vertex normal in world space (for G-buffer)
    float2 TexCoord : TEXCOORD2; // Texture coordinates
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
    
    if (isNormalMapEnabled)
    {
        output.Normal = NormalMap.Sample(Sampler, ps_input.TexCoord);
        output.Normal.x = output.Normal.x * 2.f - 1.f; // ranges from -1.f to +1.f now
        output.Normal.y = output.Normal.y * 2.f - 1.f;
        output.Normal.z = -output.Normal.z;
        //output.Normal = mul(output.Normal, World);
    }
    else
    {
        output.Normal = ps_input.WorldNormal;
    }

    output.Albedo   = Texture.Sample(Sampler, ps_input.TexCoord);
    output.Position = ps_input.WorldPosition;
    
    return output;
}