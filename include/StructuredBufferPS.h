#pragma once
#include "StructuredBuffer.h"

template<class T, size_t numStructs>
class StructuredBufferPS : public StructuredBuffer<T, numStructs>
{
	using StructuredBuffer<T, numStructs>::pStructuredBuffer;
	using StructuredBuffer<T, numStructs>::pShaderResourseView;
	using IBindable::GetContext;
public:
	using StructuredBuffer<T, numStructs>::StructuredBuffer;
	using StructuredBuffer<T, numStructs>::Update;
	using ISlot::GetBindSlot;
	using ISlot::SetBindSlot;
	
	virtual void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, pShaderResourseView.GetAddressOf()); // : register(bindSlot)
	}
};