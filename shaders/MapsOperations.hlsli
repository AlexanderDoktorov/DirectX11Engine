float3 ApplyNormalMap(const float3 normalSample, const float3x3 tanToWorld)
{
    float3 worldNormal = normalSample * 2.0f - 1.0f;
    worldNormal = normalize(mul(worldNormal, tanToWorld));
    return worldNormal;
}

// [Input] materialSpec.rgb = Ks && materialSpec.a = Ns
float4 ApplySpecularColored(float4 materialSpec, const float4 specularSample, bool useAlpha)
{
    if (any(materialSpec.rgb))
        materialSpec.rgb *= specularSample.rgb;
    else
        materialSpec.rgb = specularSample.rgb;
        
    if (useAlpha)
        materialSpec.a *= specularSample.a;
    return materialSpec;
}

// [Input] materialSpec.rgb = Ks && materialSpec.a = Ns
float4 ApplySpecularRed(float4 materialSpec, const float4 specularSample)
{
    materialSpec.a *= specularSample.r;
    return materialSpec;
}

// [Input] materialSpec.rgb = Ks && materialSpec.a = Ns
float3 ApplyDiffuseMap(float3 materialDiff, const float3 diffuseSampleColor)
{
    if (any(materialDiff))
        materialDiff *= diffuseSampleColor;
    else
        materialDiff = diffuseSampleColor;
    return materialDiff;
}

float3 ApplyIlluminationModel(uint illum, float3 ambient, float3 diffuse, float3 specular)
{
    switch (illum)
    {
        case 1:
            return saturate(diffuse); // Color on and Ambient off
        case 2:
            return saturate(ambient + diffuse); // Color on and Ambient on
        case 3:
            return saturate(ambient + diffuse + specular); // Highlight on
    }
    return float3(0.f, 1.f, 0.f); // no matching illum model - RED
}

void AlphaTest(float alpha)
{
    clip(alpha < 0.1f ? -1 : 1);
}

static const float3 ambient_default = float3(0.05f, 0.05f, 0.05f);
