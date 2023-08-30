#include "LightSource.h"

std::unique_ptr<LightConstantBuffer<LightSource::LightBuffer>> LightSource::p_light_constant_buffer{};
unsigned int LightSource::light_num = 0U;

void LightSource::SetWorldPosition(const dx::XMFLOAT3& new_Wpos)
{
	lightDesc.pos = new_Wpos;
}

dx::XMFLOAT3  LightSource::GetWorldPosition() const noexcept
{
	return lightDesc.pos;
}

const char* LightSource::ToString() const noexcept
{
	return "Light Source";
}

