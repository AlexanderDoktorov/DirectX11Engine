#pragma once
#include "Parallelogram.h"

class Box : public Parallelogram
{
	using Parallelogram::Parallelogram;
public:

	Box(Graphics& gfx, float size = 1.f) : Parallelogram(gfx, size, size, size) {}

	void Scale(float k)
	{
		Parallelogram::Scale(k, k, k);
	}
};