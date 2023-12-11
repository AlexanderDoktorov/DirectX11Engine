cbuffer transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

float4 main(float3 pos : POSITION) : SV_POSITION
{
    matrix WVP = mul(mul(World, View), Projection);
    return mul(float4(pos, 1.f), WVP);
}
