#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template <class T = DirectX::XMFLOAT4>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using IPipelineElement::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
public:
	using ConstantBuffer<T>::ConstantBuffer;

	virtual void Attach(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->PSSetConstantBuffers(0U, 1U, p_ConstantBuffer.GetAddressOf()); // : register(b0)
	}
};