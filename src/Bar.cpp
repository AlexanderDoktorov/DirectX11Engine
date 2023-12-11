#include "Bar.h"

void Bar::SetPosition(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

DirectX::XMFLOAT3 Bar::GetPosition() const noexcept
{
	return dx::XMFLOAT3(x,y,z);
}

void Bar::SetScaleX(const float& scale_x_new)
{
	scale.x = scale_x_new;
}

void Bar::SetScaleY(const float& scale_y_new)
{
	scale.y = scale_y_new;
}

void Bar::SetScaleZ(const float& scale_z_new)
{
	scale.z = scale_z_new;
}

dx::XMFLOAT3 Bar::GetScale() const noexcept
{
	return scale;
}

DirectX::XMFLOAT4 Bar::GetColor() const noexcept
{
	return color;
}

void Bar::SetColor(dx::XMFLOAT4 new_color) noexcept
{
	color = new_color;
}

const char* Bar::ToString() const noexcept
{
	return "Bar";
}
