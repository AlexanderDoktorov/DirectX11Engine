#pragma once
#include <d3d11.h>
#include "IPipelineElement.h"

class Topology : public IPipelineElement
{
public:
	Topology(const D3D11_PRIMITIVE_TOPOLOGY& topology) : Pt(topology) {}

	void Attach(Graphics& Gfx) noexcept
	{
		GetContext(Gfx)->IASetPrimitiveTopology(Pt);
	}

private:
	D3D11_PRIMITIVE_TOPOLOGY Pt;
};