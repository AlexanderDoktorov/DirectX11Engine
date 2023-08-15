#pragma once
#include "Parallelogram.h"

template <float size>
class Box : public Parallelogram<size, size, size>
{
public:
	using Parallelogram<size, size, size>::Parallelogram;
	using Parallelogram<size, size, size>::Make;
};