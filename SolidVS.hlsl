cbuffer transform : register(b0)
{
    matrix transformation;
};

float4 main(float3 pos : POSITION) : SV_POSITION
{
    return mul(float4(pos, 1.f), transformation);
}
