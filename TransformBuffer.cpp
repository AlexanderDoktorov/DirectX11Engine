#include "TransformBuffer.h"

std::unique_ptr<VertexConstantBuffer<TransformBuffer::Transformations>> TransformBuffer::Cbuff;

TransformBuffer::TransformBuffer(Graphics& Gfx, Drawable& parent) : parent(parent)
{
	if (!Cbuff) Cbuff = std::make_unique<VertexConstantBuffer<Transformations>>(Gfx);
}

void TransformBuffer::Bind(Graphics& Gfx) noexcept
{
	//												Model to World			World to View (rotates world)		View to Projection (flattens objects to fit the camera)
	auto ModelToView = parent.GetTransform() * Gfx.GetCamera().GetCameraMatrix();

	Cbuff->Update(Gfx, Transformations(dx::XMMatrixTranspose(ModelToView * Gfx.GetProjection()), dx::XMMatrixTranspose(ModelToView)));
	Cbuff->Bind(Gfx);
}