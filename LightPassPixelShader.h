#pragma once
#include "PixelShaderCommon.h"

class LightPassPixelShader : public PixelShaderCommon
{
public:
	LightPassPixelShader(Graphics& Gfx) : PixelShaderCommon(Gfx, L"LightPS.cso")
	{

	}
};



