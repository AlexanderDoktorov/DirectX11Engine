#pragma once
#include <DirectXMath.h>

class IMovable
{
public:
	virtual ~IMovable() = default; 

	virtual void SetPosition(float _x, float _y, float _z) = 0;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept = 0;

	void SetPosition(DirectX::XMFLOAT3 new_pos)
	{
		SetPosition(new_pos.x, new_pos.y, new_pos.z);
	}
	float GetX() const noexcept
	{
		return GetPosition().x;
	}
	float GetY() const noexcept
	{
		return GetPosition().y;
	}
	float GetZ() const noexcept
	{
		return GetPosition().z;
	}
};