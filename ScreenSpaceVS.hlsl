struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

struct VS_INPUT
{
    float2 position : POSITION0;
    float2 UV : TEXTCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    output.Position = float4(input.position, 0.f, 1.f);
    output.TexCoord = input.UV;

    return output;
}