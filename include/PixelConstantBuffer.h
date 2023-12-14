#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include "BindableSystem.h"

template <class T = DirectX::XMFLOAT4>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using IBindable::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
	using ConstantBuffer<T>::ISlot::GetBindSlot;
public:
	using ConstantBuffer<T>::ConstantBuffer;

	static std::shared_ptr<PixelConstantBuffer> Resolve( Graphics& Gfx,const T& data,UINT slot = 0 )
	{
		return BindableSystem::Resolve<PixelConstantBuffer>( Gfx,data,slot );
	}
	static std::shared_ptr<PixelConstantBuffer> Resolve( Graphics& Gfx,UINT slot = 0)
	{
		return BindableSystem::Resolve<PixelConstantBuffer>( Gfx,slot );
	}
	static std::string GenerateID([[maybe_unused]] const T& , UINT slot)
	{
		return GenerateID( slot );
	}
	static std::string GenerateID( UINT slot = 0 )
	{
		using namespace std::string_literals;
		return typeid(PixelConstantBuffer).name() + "#"s + std::to_string( slot );
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, p_ConstantBuffer.GetAddressOf());
	}

	virtual void Unbind(Graphics& Gfx) noexcept
	{
		ID3D11Buffer* pNullPSBuffer[1] = { nullptr };
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, pNullPSBuffer); // Unbind register(bindSlot)
	}
};