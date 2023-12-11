#pragma once
#include "IBindable.h"
#include "PixelConstantBuffer.h"
#include "IColored.h"
#include <DirectXMath.h>
#include <memory>

namespace dx = DirectX;

class ColorBuffer : public IBindable
{
public:
	ColorBuffer(Graphics& Gfx, IColored& ColoredObject);

	void Bind(Graphics& Gfx) noexcept override;

private:
	static std::unique_ptr<PixelConstantBuffer<dx::XMFLOAT4>> color_cbuffer;
	IColored& ColoredObject;
};