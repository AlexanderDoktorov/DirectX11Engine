#include "Bar.h"

void Bar::SetPosition(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

DirectX::XMFLOAT3 Bar::GetPosition() const noexcept
{
	return dx::XMFLOAT3(x,y,z);
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
