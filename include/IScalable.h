#pragma once
#include <DirectXMath.h>

class IScalable
{
public:
	virtual ~IScalable() = default;

	virtual void SetScaleX(const float& scale_x_new) = 0;
	virtual void SetScaleY(const float& scale_y_new) = 0;
	virtual void SetScaleZ(const float& scale_z_new) = 0;
	virtual DirectX::XMFLOAT3 GetScale() const noexcept = 0;


	virtual void SetScale(const DirectX::XMFLOAT3& new_scale)
	{
		SetScaleX(new_scale.x);
		SetScaleY(new_scale.y);
		SetScaleZ(new_scale.z);
	}
	virtual float GetScaleX() const noexcept
	{
		return GetScale().x;
	}
	virtual float GetScaleY() const noexcept
	{
		return GetScale().y;
	}
	virtual float GetScaleZ() const noexcept
	{
		return GetScale().z;
	}

};
