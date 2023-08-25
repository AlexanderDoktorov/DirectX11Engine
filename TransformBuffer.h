#pragma once
#include "Drawable.h"
#include "VertexConstantBuffer.h"

class TransformBuffer : public IBindable
{
public:

	TransformBuffer(Graphics& Gfx, Drawable& parent);

	virtual void Bind(Graphics& Gfx) noexcept override;

private:

	struct Transformations
	{
		dx::XMMATRIX ModelToProjection;
		dx::XMMATRIX ModelToView;

		Transformations(const dx::XMMATRIX& ModelToProjection, const dx::XMMATRIX& ModelToView)
			: ModelToProjection(ModelToProjection), ModelToView(ModelToView)
		{
		}
	};

	const Drawable& parent;
	static	std::unique_ptr<VertexConstantBuffer<Transformations>> Cbuff;
};