#pragma once
#include "IBindable.h"
#include "PixelConstantBuffer.h"
#include "IColored.h"

class ColorBuffer : public IBindable
{
public:
	ColorBuffer(Graphics& Gfx, IColored& ColoredObject) : ColoredObject(ColoredObject)
	{
		if (!color_cbuffer) color_cbuffer = std::make_unique<PixelConstantBuffer<DirectX::XMFLOAT4>>(Gfx);
	}

	void Bind(Graphics& Gfx) noexcept override
	{
		color_cbuffer->Update(Gfx, ColoredObject.GetColor());
		color_cbuffer->Bind(Gfx);
	}

private:
	std::unique_ptr<PixelConstantBuffer<dx::XMFLOAT4>> color_cbuffer;
	IColored& ColoredObject;
};