#pragma once
#include "IPipelineElement.h"
#include "PixelConstantBuffer.h"
#include "IColored.h"

class ColorBuffer : public IPipelineElement
{
public:

	ColorBuffer(Graphics& Gfx, IColored& parent) : parent(parent)
	{
		if (!color_cbuffer) color_cbuffer = std::make_unique<PixelConstantBuffer<DirectX::XMFLOAT4>>(Gfx);
	}

	void Attach(Graphics& Gfx) noexcept override
	{
		color_cbuffer->Update(Gfx, parent.GetColor());
		color_cbuffer->Attach(Gfx);
	}

private:
	std::unique_ptr<PixelConstantBuffer<dx::XMFLOAT4>> color_cbuffer;
	IColored& parent;
};