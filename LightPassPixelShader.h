#pragma once
#include "PixelShaderCommon.h"
#include "Sampler.h"

class LightPassPixelShader : public PixelShaderCommon
{
public:
	LightPassPixelShader(Graphics& Gfx) : PixelShaderCommon(Gfx, L"LightPS.cso")
	{
		pSampler = std::make_unique<Sampler>(Gfx);
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		PixelShaderCommon::Bind(Gfx);
		pSampler->Bind(Gfx);
	}

private:
	std::unique_ptr<Sampler> pSampler;
};



