#include "Shift.h"

dx::XMFLOAT3 Shift(dx::XMFLOAT3& point, float step, SHIFT_DIR dir)
{
	switch (dir)
	{	
	case SHIFT_DIR_UP:
		point.y += step;
		break;
	case SHIFT_DIR_DOWN:
		point.y -= step;
		break;
	case SHIFT_DIR_RIGHT:
		point.x += step;
		break;
	case SHIFT_DIR_LEFT:
		point.x -= step;
		break;
	case SHIFT_DIR_FORWARD:
		point.z += step;
		break;
	case SHIFT_DIR_BACK:
		point.z -= step;
		break;
	default:
		break;
	}
	return point;
}
