#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template<class T>
class DataBufferPS : public ConstantBuffer<T>
{
	using IBindable::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
	using ConstantBuffer<T>::ISlot::GetBindSlot;
	using ConstantBuffer<T>::ISlot::SetBindSlot;
	using ConstantBuffer<T>::ConstantBuffer;
	using ConstantBuffer<T>::Update;
public:
	DataBufferPS(Graphics& gfx, T& dataRef, UINT bindSlot = 0U) : ConstantBuffer<T>::ConstantBuffer(gfx), dataRef(dataRef)
	{
		SetBindSlot(bindSlot);
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		Update(Gfx, dataRef);
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, p_ConstantBuffer.GetAddressOf()); // : register(bindSlot)
	}

	virtual void Unbind(Graphics& Gfx) noexcept override
	{
		ID3D11Buffer* pNullPSDataBuffer[1] = { nullptr };
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, pNullPSDataBuffer); // Unbind register(bindSlot)
	}

private:
	T& dataRef;
};