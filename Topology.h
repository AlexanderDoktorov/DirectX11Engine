#pragma once
#include <d3d11.h>
#include "IBindable.h"

class Topology : public IBindable
{
public:
	Topology(const D3D11_PRIMITIVE_TOPOLOGY& topology) : Pt(topology) {}

	void Bind(Graphics& Gfx) noexcept
	{
		GetContext(Gfx)->IASetPrimitiveTopology(Pt);
	}

	void SetTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology)
	{
		Pt = topology;
	}

private:
	D3D11_PRIMITIVE_TOPOLOGY Pt;
};