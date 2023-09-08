#pragma once
#include "PixelShaderCommon.h"
#include <vector>

class CombinePixelShader : public PixelShaderCommon
{
public:
	CombinePixelShader(Graphics& Gfx) : PixelShaderCommon(Gfx, L"CombinePS.cso")
	{

	}

};