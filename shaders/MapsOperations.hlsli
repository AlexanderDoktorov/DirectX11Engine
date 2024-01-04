float3 ApplyNormalMap(const float3 normalSample, const float3x3 tanToWorld)
{
    float3 worldNormal = normalSample * 2.0f - 1.0f;
    worldNormal = normalize(mul(worldNormal, tanToWorld));
    return worldNormal;
}

void AlphaTest(float alpha)
{
    clip(alpha < 0.1f ? -1 : 1);
}