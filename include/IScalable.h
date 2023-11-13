#pragma once
#include <DirectXMath.h>

class IScalable
{
public:
	virtual ~IScalable() = default;

	virtual void SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new)
	{
		GetScaleRef().x = scale_x_new;
		GetScaleRef().y = scale_y_new;
		GetScaleRef().z = scale_z_new;
	}

	virtual dx::XMFLOAT3 GetScale() const noexcept
	{
		return  const_cast<IScalable*>(this)->GetScaleRef();
	}
protected:
	virtual dx::XMFLOAT3& GetScaleRef() noexcept = 0;
};
