#include "Sampler.h"
#include "BindableSystem.h"
#include "hrException.h"

Sampler::Sampler(Graphics& Gfx, UINT bindSlot) : Slotted(bindSlot)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.f;

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

std::string Sampler::GenerateID(UINT bindSlot) noexcept
{
	using namespace std::string_literals;
	return std::string(typeid(Sampler).name()).append("#").append(std::to_string(bindSlot));
}

std::shared_ptr<Sampler> Sampler::Resolve(Graphics& Gfx, UINT bindSlot) noexcept
{
	return BindableSystem::Resolve<Sampler>(Gfx, bindSlot);
}
