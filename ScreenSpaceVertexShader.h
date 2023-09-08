#pragma once
#include "VertexShaderCommon.h"
#include "Topology.h"

class ScreenSpaceVertexShader : public VertexShaderCommon
{
public:
	ScreenSpaceVertexShader(Graphics& Gfx) : VertexShaderCommon(Gfx, L"ScreenSpaceVS.cso"), topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
	{

	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		VertexShaderCommon::Bind(Gfx);
		topology.Bind(Gfx);
	}

private:
	Topology topology;
};