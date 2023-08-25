#pragma once
#include "Graphics.h"
#include "Exceptions.h"
#include <memory>

class IBindable
{
public:
	virtual ~IBindable() = default;

	virtual void Bind(Graphics& Gfx) noexcept = 0;

protected:
	ID3D11Device* GetDevice(Graphics& Gfx) { return Gfx.p_Device.Get(); }
	ID3D11DeviceContext* GetContext(Graphics& Gfx) { return Gfx.p_Context.Get(); }
};