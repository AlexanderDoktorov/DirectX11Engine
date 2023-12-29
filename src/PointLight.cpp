#include "PointLight.h"

#define POINT_LIGHT_MODEL_PATH R"(.\Models\Campanella_SP1_obj\Campanella SP1.obj)"

PointLight::PointLight(Graphics& Gfx) 
	:
	Model(Gfx, POINT_LIGHT_MODEL_PATH, 
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	),
	Light(Gfx, LIGHT_TYPE_POINT_LIGHT)
{
	TrackPosition(this->data.worldPosition);
	Reset();
}

void PointLight::SetPosition(float _x, float _y, float _z)
{
	data.worldPosition = { _x,_y,_z };
}

DirectX::XMFLOAT3 PointLight::GetPosition() const noexcept
{
	return data.worldPosition;
}

const char* PointLight::ToString() const noexcept
{
	return "Point Light Source";
}

void PointLight::Reset() noexcept
{
	Light::Reset();
}

void PointLight::Update(float dt) noexcept
{
	// TO DO
}

