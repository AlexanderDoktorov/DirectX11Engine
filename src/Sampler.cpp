#include "Sampler.h"
#include "hrException.h"

Sampler::Sampler(Graphics& Gfx, UINT bindSlot) : Slotted(bindSlot)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	CHECK_EXPR_DEFINE_HR(GetDevice(Gfx)->CreateSamplerState(&samplerDesc, &p_SamplerState));
}

void Sampler::Bind(Graphics & Gfx) noexcept
{
	GetContext(Gfx)->PSSetSamplers(GetBindSlot(), 1U, p_SamplerState.GetAddressOf());
}

void Sampler::Unbind(Graphics& Gfx) noexcept
{
	ID3D11SamplerState* pNullSampler[1] = { nullptr };
	GetContext(Gfx)->PSSetSamplers(GetBindSlot(), 1U, pNullSampler);
}
