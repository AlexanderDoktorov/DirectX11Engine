#pragma once
#include "Drawable.h"
#include "VertexConstantBuffer.h"
#include <DirectXMath.h>
#include "BindableSystem.h"

class TransformBuffer : public IBindable
{
public:
	TransformBuffer(Graphics& Gfx, Drawable& object);
	virtual void Bind(Graphics& Gfx) noexcept override;

private:
	struct Transformations
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;

		Transformations(const DirectX::XMMATRIX& World, const DirectX::XMMATRIX& View, const DirectX::XMMATRIX& Projection)
			: World(World), View(View), Projection(Projection)
		{
		}
	};

	const Drawable& object;
	static	std::unique_ptr<VertexConstantBuffer<Transformations>> Cbuff;
};