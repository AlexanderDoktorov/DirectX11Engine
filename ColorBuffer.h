#pragma once
#include "IBindable.h"
#include "PixelConstantBuffer.h"
#include "IColored.h"

class ColorBuffer : public IBindable
{
public:
	ColorBuffer(Graphics& Gfx, IColored& ColoredObject);

	void Bind(Graphics& Gfx) noexcept override;

private:
	static std::unique_ptr<PixelConstantBuffer<dx::XMFLOAT4>> color_cbuffer;
	IColored& ColoredObject;
};