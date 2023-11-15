cbuffer transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_OUT
{
    float4 Position : SV_POSITION; // Position in homogeneous clip space
    float3 WorldPosition : WORLDPOS0; // Vertex position in world space (for G-buffer)
    float2 TexCoord : TEXCOORD0; // Texture coordinates
};

VS_OUT main(float3 pos : POSITION, float2 tc : TEXCOORD0)
{
    VS_OUT output;

    // Transform the vertex position to homogeneous clip space
    output.Position = mul(float4(pos, 1.0f), World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    // Pass other data to the pixel shader
    output.WorldPosition = mul(float4(pos, 1.0f), World).xyz;
    output.TexCoord = tc;
    
    return output;
}