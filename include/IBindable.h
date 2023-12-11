#pragma once
#include <d3d11.h>

class Graphics;

class GraphicsChild
{
protected:
	virtual ~GraphicsChild() = default;
	ID3D11Device* GetDevice(Graphics& Gfx);
	ID3D11DeviceContext* GetContext(Graphics& Gfx);
};

struct IBindable : virtual public GraphicsChild
{
public:
	virtual ~IBindable() = default;
	virtual void Bind(Graphics& Gfx) noexcept = 0;
};

struct IUnbindable : virtual public GraphicsChild
{
public:
	virtual ~IUnbindable() = default;
	virtual void Unbind(Graphics& Gfx) noexcept = 0;
};