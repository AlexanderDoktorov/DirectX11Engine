#pragma once
#include <DirectXMath.h>

class IMovable
{
public:
	virtual ~IMovable() = default; 

	virtual void SetPosition(float _x, float _y, float _z) = 0;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept = 0;
};