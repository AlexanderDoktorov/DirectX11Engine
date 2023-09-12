#pragma once
#include <DirectXMath.h>

class IScalable
{
public:
	virtual ~IScalable() = default;

	virtual void SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new) = 0;
	void SetScale(const DirectX::XMFLOAT3& scale_XYZ)
	{
		SetScale(scale_XYZ.x, scale_XYZ.y, scale_XYZ.z);
	}

	virtual dx::XMFLOAT3 GetScale() const noexcept = 0;

	float GetScaleX() const noexcept
	{
		return GetScale().x;
	}
	float GetScaleY() const noexcept
	{
		return GetScale().y;
	}
	float GetScaleZ() const noexcept
	{
		return GetScale().z;
	}
};