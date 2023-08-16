#include "Mouse.h"

void Mouse::OnDeltaRaw(const LONG& lLastX, const LONG& lLastY)
{
	rawData.rawX += lLastX;
	rawData.rawY += lLastY;
}
