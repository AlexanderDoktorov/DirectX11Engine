#pragma once
#include "Camera.h"

class Player
{
public:
	Player() = default;

	float GetSpeed() const noexcept;

private:
	float speed = 0.2f;
};