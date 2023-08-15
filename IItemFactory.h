#pragma once
#include "Items.h"

class IItemFactory
{
public:
	 template <float width, float height, float length>
	 static void CreateParallelogramm(Graphics& gfx, Parallelogram<width, height, length>** ppParallelogramm)
	 {
		 Parallelogram<width, height, length>::Make(gfx);
		 *ppParallelogramm = new Parallelogram<width, height, length>(gfx);
	 }

	 template <float size>
	 static void CreateBox(Graphics& gfx, Box<size>** ppBox)
	 {
		 Box<size>::Make(gfx);
		 *ppBox = new Box<size>(gfx);
	 }
};
