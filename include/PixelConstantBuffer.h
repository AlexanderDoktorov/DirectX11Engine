#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template <class T = dx::XMFLOAT4>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using IBindable::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
	using ConstantBuffer<T>::ISlot::GetBindSlot;
public:
	using  ConstantBuffer<T>::ConstantBuffer;

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, p_ConstantBuffer.GetAddressOf()); // : register(bindSlot)
	}
};