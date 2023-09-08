#pragma once
#include "VertexShaderCommon.h"
#include "Topology.h"

class ScreenSpaceVertexShader : public VertexShaderCommon
{
public:
	ScreenSpaceVertexShader(Graphics& Gfx) : VertexShaderCommon(Gfx, L"ScreenSpaceVS.cso")
	{

	}
};