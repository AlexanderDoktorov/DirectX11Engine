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
float Speculate(float3 fragWorldNormal, float3 fragWorldPos, float3 camWorldPos, float3 dirToL, float shininess)
{
    const float3 viewDir = normalize(camWorldPos - fragWorldPos);
    return pow(saturate(dot(viewDir, reflect(-dirToL, fragWorldNormal))), shininess);
}

// diffAngle
float Diffusate(float3 fragWorldNormal, float3 dirToL)
{
    return max(0.0f, dot(dirToL, fragWorldNormal));
}