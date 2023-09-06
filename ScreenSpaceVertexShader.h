#pragma once
#include "VertexShader.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "VertexShaderCommon.h"

class ScreenSpaceVertexShader : public VertexShaderCommon
{
public:
	ScreenSpaceVertexShader(Graphics& Gfx) : VertexShaderCommon(Gfx, L"ScreenSpaceVS.cso")
	{
		CreateTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
};