#pragma once

#include "PixelShaderCommon.h"
#include "Sampler.h"
#include <vector>

class CombinePixelShader : public PixelShaderCommon
{
public:
	CombinePixelShader(Graphics& Gfx) : PixelShaderCommon(Gfx, L"CombinePS.cso")
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