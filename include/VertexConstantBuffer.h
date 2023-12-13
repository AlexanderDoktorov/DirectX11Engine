#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template <class T = DirectX::XMMATRIX>
class VertexConstantBuffer : public ConstantBuffer<T, shader_type_vertex>
{
	using IBindable::GetContext;
	using ConstantBuffer<T, shader_type_vertex>::p_ConstantBuffer;
	using ISlot::GetBindSlot;
public:
	using ConstantBuffer<T, shader_type_vertex>::ConstantBuffer;

	virtual void Bind(Graphics& Gfx) noexcept override
	{								
		GetContext(Gfx)->VSSetConstantBuffers(GetBindSlot(), 1U, p_ConstantBuffer.GetAddressOf()); // : register(b0)
	}

	virtual void Unbind(Graphics& Gfx) noexcept
	{								
		ID3D11Buffer* pNullVSBuffer[1] = { nullptr };
		GetContext(Gfx)->VSSetConstantBuffers(GetBindSlot(), 1U, pNullVSBuffer); // Unbind register(bindSlot)
	}
};