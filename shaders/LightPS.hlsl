// Texture resources containing G-buffer data
Texture2D<float4> GBufferPosition   : register(t0);
Texture2D<float4> GBufferNormal     : register(t1);
Texture2D<float4> GBufferAlbedo     : register(t2);
SamplerState      sampleState       : register(s0);

struct LightDesc
{
    float3 pos;
    float3 diffuseColor;
    float diffuseIntensity;
    float Catt;
    float Latt;
    float Qatt;
};

cbuffer CameraBuffer : register(b0)
{
    float3 viewPos;
};

// Constant buffer for lighting parameters
cbuffer LightingPassPSConstants : register(b1)
{
    LightDesc ligthParams;
};

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

static float3 ambient = float3(0.1f, 0.1f, 0.1f);
static float specularStrength = 1.f;
static float specularPower = 256;

float4 ApplyLightImpact(float3 world_normal, float3 world_pos, float4 materialcolor)
{
    const float3 vToL       = ligthParams.pos - world_pos;
    const float3 distToL    = length(vToL);
    const float3 dirToL     = normalize(vToL);
    const float  att        = 1.0f / (ligthParams.Catt + ligthParams.Latt * distToL + ligthParams.Qatt * (distToL * distToL));
    const float3 diffuse    = ligthParams.diffuseColor * ligthParams.diffuseIntensity * att * max(0.0f, dot(dirToL, world_normal));
    
    // specular
    const float3 viewDir = normalize(viewPos - world_pos);
    const float3 reflectDir = reflect(-dirToL, world_normal);
    const float spec = pow(saturate(dot(viewDir, reflectDir)), specularPower);
    const float3 specular = specularStrength * spec * ligthParams.diffuseColor;

    return float4(saturate((diffuse + ambient + specular) * materialcolor.rgb), 1.0f);
}


float4 main(in PS_INPUT input) : SV_Target0
{
    float3 worldPosition = GBufferPosition.Sample(sampleState, input.TexCoord).xyz;
    float3 worldNormal = GBufferNormal.Sample(sampleState, input.TexCoord).xyz;
    float4 materialColor = GBufferAlbedo.Sample(sampleState, input.TexCoord);
    
    return ApplyLightImpact(worldNormal, worldPosition, materialColor);
}