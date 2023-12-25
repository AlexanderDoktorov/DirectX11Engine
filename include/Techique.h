#pragma once
#include <IBindable.h>
#include <vector>

// Future expectations:
// Model can accept techniques and use them to build pipeline for drawing
// It can has vertex shader, pixel shader, input layout and others
// e.g. : wireframe technique would be when D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP is set
// something like that
class Technique : public IBindable
{
public:
	Technique(std::vector<IBindable*> bindables) :
		bindablePtrs(std::move(bindables))
	{
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		for (auto& pBindable : bindablePtrs)
		{
			pBindable->Bind(Gfx);
		}
	}

private:
	std::vector<IBindable*> bindablePtrs;
};