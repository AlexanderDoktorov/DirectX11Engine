#include "Bar.h"

void Bar::SetPosition(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

DirectX::XMFLOAT3 Bar::GetPosition() const noexcept
{
	return dx::XMFLOAT3(x,y,z);
}

void Bar::SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new)
{
	scale_x = scale_x_new;
	scale_y = scale_y_new;
	scale_z = scale_z_new;
}

dx::XMFLOAT3 Bar::GetScale() const noexcept
{
	return dx::XMFLOAT3(scale_x,scale_y,scale_z);
}
