#pragma once
#include "IPipelineElement.h"
#include <wrl.h>

class Sampler : public IPipelineElement
{
public:

	Sampler(Graphics& Gfx);

	virtual void Attach(Graphics& Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> p_SamplerState;
};