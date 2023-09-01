#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template <class T>
class LightConstantBuffer : public ConstantBuffer<T>
{
	using IBindable::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
public:
	using  ConstantBuffer<T>::ConstantBuffer;

	UINT nStartSlot() const noexcept
	{
		return 1U;
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->PSSetConstantBuffers(nStartSlot(), 1U, p_ConstantBuffer.GetAddressOf()); // : register(b1)
	}
};