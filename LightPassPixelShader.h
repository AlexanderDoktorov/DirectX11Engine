#pragma once
#include "PixelShader.h"
#include "Sampler.h"
#include <vector>

class LightPassPixelShader : public IBindable
{
public:
	LightPassPixelShader(Graphics& Gfx)
	{
		pPixelShader = std::make_unique<PixelShader>(Gfx, L"LightPS.cso");
		pSampler = std::make_unique<Sampler>(Gfx);
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		pPixelShader->Bind(Gfx);
		pSampler->Bind(Gfx);
	}

private:
	std::unique_ptr<PixelShader> pPixelShader;
	std::unique_ptr<Sampler> pSampler;
};