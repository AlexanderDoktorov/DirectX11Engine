#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template<class T>
class DataBufferPS : public ConstantBuffer<T, shader_type_pixel>
{
	using IBindable::GetContext;
	using ConstantBuffer<T, shader_type_pixel>::p_ConstantBuffer;
	using ConstantBuffer<T, shader_type_pixel>::ISlot::GetBindSlot;
	using ConstantBuffer<T, shader_type_pixel>::ISlot::SetBindSlot;
	using ConstantBuffer<T, shader_type_pixel>::ConstantBuffer;
	using ConstantBuffer<T, shader_type_pixel>::Update;
public:
	DataBufferPS(Graphics& gfx, T& dataRef, UINT bindSlot = 0U) : ConstantBuffer<T, shader_type_pixel>(gfx), dataRef(dataRef)
	{
		SetBindSlot(bindSlot);
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		Update(Gfx, dataRef);
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, p_ConstantBuffer.GetAddressOf()); // : register(bindSlot)
	}

	virtual void Unbind(Graphics& Gfx) noexcept
	{
		ID3D11Buffer* pNullPSDataBuffer[1] = { nullptr };
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, pNullPSDataBuffer); // Unbind register(bindSlot)
	}

private:
	T& dataRef;
};