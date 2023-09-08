#pragma once
#include "ISlot.h"
#include "IBindable.h"
#include <wrl.h>

class Sampler : public ISlot, public IBindable
{
public:
	Sampler(Graphics& Gfx);

	// IBindable
	void Bind(Graphics& Gfx) noexcept override;

	// ISlot
	void SetBindSlot(UINT slot) noexcept override;
	UINT GetBindSlot() const noexcept override;

private:
	UINT bindSlot = 0U;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> p_SamplerState;
};