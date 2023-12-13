#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>

template <class T = DirectX::XMFLOAT4>
class PixelConstantBuffer : public ConstantBuffer<T, shader_type_pixel>
{
	using IBindable::GetContext;
	using ConstantBuffer<T, shader_type_pixel>::p_ConstantBuffer;
	using ConstantBuffer<T, shader_type_pixel>::ISlot::GetBindSlot;
public:
	using ConstantBuffer<T, shader_type_pixel>::ConstantBuffer;

	virtual void Unbind(Graphics& Gfx) noexcept
	{
		ID3D11Buffer* pNullPSBuffer[1] = { nullptr };
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, pNullPSBuffer); // Unbind register(bindSlot)
	}
};