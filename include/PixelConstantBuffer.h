#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template <class T = DirectX::XMFLOAT4>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using IBindable::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
	using ConstantBuffer<T>::ISlot::GetBindSlot;
public:
	using ConstantBuffer<T>::ConstantBuffer;

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, p_ConstantBuffer.GetAddressOf()); // : register(bindSlot)
	}

	virtual void Unbind(Graphics& Gfx) noexcept override
	{
		ID3D11Buffer* pNullPSBuffer[1] = { nullptr };
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, pNullPSBuffer); // Unbind register(bindSlot)
	}
};