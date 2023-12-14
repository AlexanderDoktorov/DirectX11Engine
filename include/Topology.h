#pragma once
#include <d3d11.h>
#include "BindableSystem.h"

class Topology : public IBindable
{
public:
	Topology(const D3D11_PRIMITIVE_TOPOLOGY& topology) : Pt(topology) {}
	Topology(Graphics&, const D3D11_PRIMITIVE_TOPOLOGY& topology) : Pt(topology) {}

	void Bind(Graphics& Gfx) noexcept
	{
		GetContext(Gfx)->IASetPrimitiveTopology(Pt);
	}

	void SetTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology)
	{
		Pt = topology;
	}

	static std::shared_ptr<Topology> Resolve(Graphics& Gfx, const D3D11_PRIMITIVE_TOPOLOGY& topology)
	{
		return BindableSystem::Resolve<Topology>(Gfx, topology);
	}

	static std::string GenerateID(const D3D11_PRIMITIVE_TOPOLOGY& topology) noexcept
	{
		using namespace std::string_literals;
		return typeid(topology).name() + "#ptid:"s + std::to_string(topology);
	}

private:
	D3D11_PRIMITIVE_TOPOLOGY Pt;
};