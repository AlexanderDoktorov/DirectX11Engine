#pragma once
#include "Items.h"

class IItemFactory
{
public:
	virtual ~IItemFactory() = default;

	virtual void CreateParallelogramm(Graphics& gfx, Parallelogram** ppParallelogramm)
	{
		Parallelogram::Make(gfx);
		*ppParallelogramm = new Parallelogram(gfx);
	}

	virtual void CreateBox(Graphics& gfx, Box** ppBox)
	{
		Box::Make(gfx);
		*ppBox = new Box(gfx);
	}
};
