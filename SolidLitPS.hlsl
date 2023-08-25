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

cbuffer colorbuffer : register(b0)
{
    float4 materialcolor;
};

float4 main(float3 normal : NORMAL0, float3 world_pos : WORLDPOS) : SV_TARGET
{
	// fragment to light vector data
    const float3 vToL = light_position - world_pos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (Catt + Latt * distToL + Qatt * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, normal));
	// final color
    return float4(saturate((diffuse + ambient) * materialcolor.rgb), 1.0f);
}