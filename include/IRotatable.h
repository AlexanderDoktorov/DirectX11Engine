#pragma once
#include <DirectXMath.h>

class IRotatable
{
public:
	virtual ~IRotatable() = default;

	virtual float GetRoll()			const noexcept = 0;
	virtual float GetYaw()			const noexcept = 0;
	virtual float GetPitch()		const noexcept = 0;

	virtual void SetRotation(const float& roll, const float& pitch, const float& yaw) = 0;
};
