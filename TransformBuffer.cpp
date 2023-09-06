#include "TransformBuffer.h"

std::unique_ptr<VertexConstantBuffer<TransformBuffer::Transformations>> TransformBuffer::Cbuff;

TransformBuffer::TransformBuffer(Graphics& Gfx, Drawable& parent) : parent(parent)
{
	if (!Cbuff) Cbuff = std::make_unique<VertexConstantBuffer<Transformations>>(Gfx);
}

void TransformBuffer::Bind(Graphics& Gfx) noexcept
{
	//												Model to World			World to View (rotates world)		View to Projection (flattens objects to fit the camera)
	auto World		= parent.GetTransform();
	auto View		= Gfx.GetCamera().GetCameraMatrix();
	auto Projection = Gfx.GetProjection();

	Cbuff->Update(Gfx, Transformations(dx::XMMatrixTranspose(World), dx::XMMatrixTranspose(View), dx::XMMatrixTranspose(Projection)));
	Cbuff->Bind(Gfx);
}