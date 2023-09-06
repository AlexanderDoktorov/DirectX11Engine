// Transforms coordinates
cbuffer transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL0;
};

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float3 WorldPosition : TEXCOORD0; // Vertex position in world space (for G-buffer)
    float3 WorldNormal : TEXCOORD1; // Vertex normal in world space (for G-buffer)
    float2 TexCoord : TEXCOORD2; // Texture coordinates
};

// Vertex Shader function
VS_OUT vs_main(VS_INPUT input)
{
    VS_OUT output;

    // Transform the vertex position to homogeneous clip space
    output.Position = mul(float4(input.Position, 1.0f), World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    // Pass other data to the pixel shader
    output.WorldPosition = mul(float4(input.Position, 1.0f), World).xyz;
    output.WorldNormal = normalize(mul(float4(input.Normal, 1.0f), World).xyz);
    output.TexCoord = input.TexCoord;

    return output;
}