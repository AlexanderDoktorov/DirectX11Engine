#pragma once
#include "Exceptions.h"
#include "GraphicsChild.h"

class IBindable : virtual public GraphicsChild
{
public:
	virtual ~IBindable() = default;
	virtual void Bind(Graphics& Gfx) noexcept = 0;
};

class IUnbindable : virtual public GraphicsChild
{
public:
	virtual ~IUnbindable() = default;
	virtual void Unbind(Graphics& Gfx) noexcept = 0;
};