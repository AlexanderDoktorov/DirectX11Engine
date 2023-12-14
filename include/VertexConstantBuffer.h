#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include "BindableSystem.h"

template <class T = DirectX::XMMATRIX>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using IBindable::GetContext;
	using ConstantBuffer<T>::p_ConstantBuffer;
	using ISlot::GetBindSlot;
public:
	using ConstantBuffer<T>::ConstantBuffer;

	static std::shared_ptr<VertexConstantBuffer> Resolve( Graphics& Gfx,const T& data,UINT slot = 0 )
	{
		return BindableSystem::Resolve<VertexConstantBuffer>( Gfx,data,slot );
	}
	static std::shared_ptr<VertexConstantBuffer> Resolve( Graphics& Gfx,UINT slot = 0)
	{
		return BindableSystem::Resolve<VertexConstantBuffer>( Gfx,slot );
	}
	static std::string GenerateID([[maybe_unused]] const T& , UINT slot)
	{
		return GenerateID( slot );
	}
	static std::string GenerateID( UINT slot = 0 )
	{
		using namespace std::string_literals;
		return typeid(VertexConstantBuffer).name() + "#"s + std::to_string( slot );
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{								
		GetContext(Gfx)->VSSetConstantBuffers(GetBindSlot(), 1U, p_ConstantBuffer.GetAddressOf());
	}
	virtual void Unbind(Graphics& Gfx) noexcept
	{								
		ID3D11Buffer* pNullVSBuffer[1] = { nullptr };
		GetContext(Gfx)->VSSetConstantBuffers(GetBindSlot(), 1U, pNullVSBuffer); // Unbind register(bindSlot)
	}
};