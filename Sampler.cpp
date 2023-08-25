#include "Sampler.h"

Sampler::Sampler(Graphics& Gfx)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	CHECK_HR ( GetDevice(Gfx)->CreateSamplerState(&samplerDesc, &p_SamplerState) );
}

void Sampler::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->PSSetSamplers(0U, 1U, p_SamplerState.GetAddressOf());
}
