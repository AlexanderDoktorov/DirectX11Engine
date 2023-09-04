#pragma once
#include "IBindable.h"
#include <wrl.h>

class Sampler : public IBindable
{
public:
	Sampler(Graphics& Gfx);

	virtual void Bind(Graphics& Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> p_SamplerState;
};