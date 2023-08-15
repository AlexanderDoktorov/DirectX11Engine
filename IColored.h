#pragma once
#include <DirectXMath.h>

class IColored
{
public:
	virtual DirectX::XMFLOAT4 GetColor() const noexcept = 0;
	virtual void SetColor(dx::XMFLOAT4 new_color) noexcept = 0;
};