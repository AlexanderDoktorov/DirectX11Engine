#pragma once
#include <DirectXMath.h>

class IColored
{
public:
	IColored() = default;
	virtual DirectX::XMFLOAT4 GetColor() const noexcept
	{
		return color;
	}
	virtual void SetColor(dx::XMFLOAT4 new_color) noexcept
	{
		color = new_color;
	}
private:
	dx::XMFLOAT4 color = { 1.f,1.f,1.f,1.f };
};