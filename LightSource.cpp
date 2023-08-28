#include "LightSource.h"

void LightSource::SetWorldPosition(const dx::XMFLOAT3& new_Wpos)
{
	lightData.pos = new_Wpos;
}

dx::XMFLOAT3  LightSource::GetWorldPosition() const noexcept
{
	return lightData.pos;
}

const char* LightSource::ToString() const noexcept
{
	return "Light Source";
}
