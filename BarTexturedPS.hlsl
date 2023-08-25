// simple function
// Input - 2d coords -> output -> 4d coords

Texture2D CubeTexture : register(t0);
SamplerState CubeSamplerState : register(s0);

cbuffer lightbuffer : register(b1)
{
    float3 light_position;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float Catt;
    float Latt;
    float Qatt;
};

float4 ps_main(float2 UVs : TEXCOORD0, float3 normal : NORMAL0, float3 world_pos : WORLDPOS) : SV_TARGET
{
    float4 texture_color = CubeTexture.Sample(CubeSamplerState, UVs);
	// fragment to light vector data
    const float3 vToL = light_position - world_pos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (Catt + Latt * distToL + Qatt * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, normal));
	// final color
    return float4(saturate((diffuse + ambient) * texture_color.rgb), 1.0f);
}