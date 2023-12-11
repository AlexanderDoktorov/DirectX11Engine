struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

PS_INPUT main(in uint VertexIdx : SV_VertexID)
{
    PS_INPUT output;

    if (VertexIdx == 0)
    {
        output.Position = float4(-1.0f, 1.0f, 1.0f, 1.0f);
        output.TexCoord = float2(0.0f, 0.0f);
    }
    else if (VertexIdx == 1)
    {
        output.Position = float4(3.0f, 1.0f, 1.0f, 1.0f);
        output.TexCoord = float2(2.0f, 0.0f);
    }
    else
    {
        output.Position = float4(-1.0f, -3.0f, 1.0f, 1.0f);
        output.TexCoord = float2(0.0f, 2.0f);
    }

    return output;
}