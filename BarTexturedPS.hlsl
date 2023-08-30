// simple function
// Input - 2d coords -> output -> 4d coords

Texture2D CubeTexture : register(t0);
SamplerState CubeSamplerState : register(s0);

struct LightDesc
{
    float3 light_position;
    float3 diffuseColor;
    float diffuseIntensity;
    float Catt;
    float Latt;
    float Qatt;
};

static const float3 ambient = float3(0.1f, 0.1f, 0.1f);
static const uint MAX_NUM_LIGHTS = 5;

cbuffer lightbuffer : register(b1)
{
    uint NUM_LIGHTS;
    LightDesc lightDescs[MAX_NUM_LIGHTS];
};

cbuffer colorbuffer : register(b0)
{
    float4 materialcolor;
};

float4 ps_main(float2 UVs : TEXCOORD0, float3 normal : NORMAL0, float3 world_pos : WORLDPOS) : SV_TARGET
{
    float4 texture_color = CubeTexture.Sample(CubeSamplerState, UVs);
    float3 total_diffuse = float3(0, 0, 0);

    for (uint i = 0; i < NUM_LIGHTS; ++i)
    {
        const float3 vToL = lightDescs[i].light_position - world_pos;
        const float3 distToL = length(vToL);
        const float3 dirToL = normalize(vToL);
        const float att = 1.0f / (lightDescs[i].Catt + lightDescs[i].Latt * distToL + lightDescs[i].Qatt * (distToL * distToL));
        const float3 diffuse = lightDescs[i].diffuseColor * lightDescs[i].diffuseIntensity * att * max(0.0f, dot(dirToL, normal));
        total_diffuse += diffuse;
    }
    return float4(saturate((total_diffuse + ambient) * texture_color.rgb), 1.0f);
}