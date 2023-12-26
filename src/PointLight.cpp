#include "PointLight.h"

PointLight::PointLight(Graphics& Gfx) 
	:
	Model(Gfx, R"(.\Models\bulb\bulb.obj)", 
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	),
	LightDesc(Gfx, LIGHT_TYPE_POINT_LIGHT)
{
	Reset();
}

void PointLight::SetPosition(float _x, float _y, float _z)
{
	Model::SetRootTransform(dx::XMMatrixTranslation(_x, _y, _z));
	data.worldPosition = { _x,_y,_z };
}

DirectX::XMFLOAT3 PointLight::GetPosition() const noexcept
{
	//DirectX::XMVECTOR vCenter = DirectX::XMVectorSet(0.f,0.f,0.f,0.f);
	//vCenter = DirectX::XMVector3Transform(vCenter, GetTransform());
	//DirectX::XMFLOAT3 Position3D;
	//DirectX::XMStoreFloat3(&Position3D, vCenter);
	return data.worldPosition;
}

const char* PointLight::ToString() const noexcept
{
	return "Point Light Source";
}

void PointLight::Reset() noexcept
{
	LightDesc::Reset();
	Model::SetRootTransform(
		dx::XMMatrixTranslation
		(
			data.worldPosition.x, 
			data.worldPosition.y, 
			data.worldPosition.z
		)
	);
}

void PointLight::Update(float dt) noexcept
{
	// TO DO
}

