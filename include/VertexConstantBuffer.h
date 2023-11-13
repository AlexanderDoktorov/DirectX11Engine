#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template <class T = DirectX::XMMATRIX>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using IBindable::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
public:
	using ConstantBuffer<T>::ConstantBuffer;

	virtual void Bind(Graphics& Gfx) noexcept override
	{								
		GetContext(Gfx)->VSSetConstantBuffers(0U, 1U, p_ConstantBuffer.GetAddressOf()); // : register(b0)
	}
};