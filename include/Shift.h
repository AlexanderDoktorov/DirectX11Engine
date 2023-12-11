#pragma once
#include <DirectXMath.h>

namespace dx = DirectX;

enum SHIFT_DIR
{
	SHIFT_DIR_UP,
	SHIFT_DIR_DOWN,
	SHIFT_DIR_RIGHT,
	SHIFT_DIR_LEFT,
	SHIFT_DIR_FORWARD,
	SHIFT_DIR_BACK
};

dx::XMFLOAT3 Shift(dx::XMFLOAT3& point, float step, SHIFT_DIR dir);