#pragma once
#include "StructuredBuffer.h"

template<class T, size_t numStructs>
class StructuredBufferVS :  StructuredBuffer<T, numStructs>
{
	using StructuredBuffer<T, numStructs>::pStructuredBuffer;
	using StructuredBuffer<T, numStructs>::pShaderResourseView;
	using ISlot::GetBindSlot;
	using IBindable::GetContext;
public:
	using StructuredBuffer<T, numStructs>::StructuredBuffer;
	
	virtual void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->VSSetShaderResources(GetBindSlot(), 1U, pShaderResourseView.GetAddressOf()); // : register(bindSlot)
	}
};