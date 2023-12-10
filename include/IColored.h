#pragma once
#include <DirectXMath.h>

class IColored
{
public:
	IColored() = default;

	virtual DirectX::XMFLOAT4 GetColor() const noexcept = 0;
	virtual void SetColor(DirectX::XMFLOAT4 new_color) noexcept = 0;
};