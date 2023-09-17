#pragma once
#include "Drawable.h"
#include "VertexConstantBuffer.h"

class TransformBuffer : public IBindable
{
public:

	TransformBuffer(Graphics& Gfx, Drawable& object);

	virtual void Bind(Graphics& Gfx) noexcept override;

private:

	struct Transformations
	{
		dx::XMMATRIX World;
		dx::XMMATRIX View;
		dx::XMMATRIX Projection;

		Transformations(const dx::XMMATRIX& World, const dx::XMMATRIX& View, const dx::XMMATRIX& Projection)
			: World(World), View(View), Projection(Projection)
		{
		}
	};

	const Drawable& object;
	static	std::unique_ptr<VertexConstantBuffer<Transformations>> Cbuff;
};