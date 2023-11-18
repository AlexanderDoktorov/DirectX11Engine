#pragma once
#include "Bar.h"

class Box : public Bar
{
	using Bar::Bar;
public:

	Box(Graphics& gfx, float size = 1.f) : Bar(gfx, size, size, size) {}

	void SetScale(float k)
	{
		Bar::SetScale({ k, k, k });
	}
};