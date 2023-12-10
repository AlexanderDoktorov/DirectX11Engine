cbuffer transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_IN
{
    float3 mPosition : POSITION0;
    float3 mNormal : NORMAL0;
};

struct VS_OUT
{
    float4 Position     : SV_POSITION; // Position in homogeneous clip space
    float4 wPosition    : POSITION0; // Vertex position in world space (for G-buffer)
    float3 wNormal      : NORMAL0;
};
    
VS_OUT main(VS_IN input)
{
    VS_OUT output;

    // Transform the vertex position to homogeneous clip space
    output.Position = mul(float4(input.mPosition, 1.0f), World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    // Pass other data to the pixel shader
    output.wPosition = mul(float4(input.mPosition, 1.0f), World);
    output.wNormal = normalize(mul(float4(input.mNormal, 0.0f), World)).xyz;
    
    return output;
}