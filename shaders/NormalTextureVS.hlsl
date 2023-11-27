cbuffer transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

cbuffer materialID : register(b1)
{
    uint materialID;
};

struct VS_IN
{
    float3 mPosition : POSITION0;
    float2 textCoord : TEXCOORD0;
    float3 mNormal   : NORMAL0;
    float3 mTangent  : TANGENT0;
    float3 mBintangent : BITANGENT0;
};

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
    
VS_OUT vs_main(VS_IN input)
{
    VS_OUT output;

    // Transform the vertex position to homogeneous clip space
    output.Position = mul(float4(input.mPosition, 1.0f), World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    // Pass other data to the pixel shader
    output.wPosition = mul(float4(input.mPosition, 1.0f), World);
    output.wNormal = normalize(mul(float4(input.mNormal, 0.0f), World)).xyz;
    output.wTangent = normalize(mul(float4(input.mTangent, 0.0f), World)).xyz;
    output.wBitangent = normalize(mul(float4(input.mBintangent, 0.0f), World)).xyz;
    output.textCoord = input.textCoord;
    
    // Material
    output.materialID = materialID;
    return output;
}