#include "ColorBuffer.h"

std::unique_ptr<PixelConstantBuffer<dx::XMFLOAT4>> ColorBuffer::color_cbuffer{};

ColorBuffer::ColorBuffer(Graphics& Gfx, IColored& ColoredObject) : ColoredObject(ColoredObject)
{
	if (!color_cbuffer) color_cbuffer = std::make_unique<PixelConstantBuffer<DirectX::XMFLOAT4>>(Gfx);
}

void ColorBuffer::Bind(Graphics& Gfx) noexcept
{
	color_cbuffer->Update(Gfx, ColoredObject.GetColor());
	color_cbuffer->Bind(Gfx);
}