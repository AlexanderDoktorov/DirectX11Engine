#pragma once
#include "Exceptions.h"
#include "GraphicsChild.h"

class IBindable : virtual public GraphicsChild
{
public:
	virtual ~IBindable() = default;

	virtual void Bind(Graphics& Gfx) noexcept = 0;
};