#pragma once
#include <DirectXMath.h>

class IScalable
{
public:
	virtual ~IScalable() = default;

	virtual void SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new) = 0;
	virtual void SetScale(const float& scale_new)
	{
		SetScale(scale_new, scale_new, scale_new);
	}
	virtual dx::XMFLOAT3 GetScale() const noexcept = 0;
};