struct LightDesc
{
	// World position
    float3 worldPosition;
    // Color
    float3 ambientColor;
    float3 diffuseColor;
    float3 specularColor;
    // Intensity
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    // Attenuation
    float Catt;
    float Latt;
    float Qatt;
	/// Spotlight
    float3 spotlightDirection;
    float  spotlightAngle;
	// TypeId
    uint typeId;
};

static const uint LIGHT_TYPE_POINT_LIGHT = 0;
static const uint LIGHT_TYPE_SPOTLIGHT = 1;
static const uint LIGHT_TYPE_DIRECTIONAL = 2;

struct LightInfo
{
    float3 vToL;
    float distToL;
    float3 dirToL;
};

LightInfo BuildLightInfo(float3 lightWorldPos, float3 fragWorldPos)
{
    LightInfo li = (LightInfo) 0;
    li.vToL     = lightWorldPos - fragWorldPos;
    li.distToL  = length(li.vToL);
    li.dirToL   = normalize(li.vToL);
    return li;
}

float Attenuate(float Catt, float Latt, float Qatt, float distToL)
{
    return 1.0f / (Catt + Latt * distToL + Qatt * (distToL * distToL));
}

// specAngle
float Speculate(float3 fragWorldNormal, float3 fragWorldPos, float3 camWorldPos, float3 dirToL, float specularPower)
{
    const float3 viewDir = normalize(camWorldPos - fragWorldPos);
    return pow(max(0.f, (dot(viewDir, reflect(-dirToL, fragWorldNormal)))), specularPower);
}

// diffAngle
float Diffusate(float3 fragWorldNormal, float3 dirToL)
{
    return max(0.0f, dot(dirToL, fragWorldNormal));
}

float Diffusate(float3 spotlightDirection, float3 dirToL, float spotlightAngle)
{
    float theta = saturate(dot(spotlightDirection, dirToL));
    if (theta > cos(spotlightAngle))
        return theta;
    return 0;
}