#pragma once
#include "ISlot.h"
#include "BindableSystem.h"
#include <string>
#include <wrl.h>

class Sampler : public Slotted, public IBindable, public IUnbindable
{
public:
	Sampler(Graphics& Gfx, UINT bindSlot = 0U);

	// IBindable
	void Bind(Graphics& Gfx) noexcept override;
	void Unbind(Graphics& Gfx) noexcept override;

	static std::string GenerateID(UINT bindSlot) noexcept;
	static std::shared_ptr<Sampler> Resolve(Graphics& Gfx, UINT bindSlot = 0U) noexcept;

private:
	UINT bindSlot = 0U;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> p_SamplerState;
};