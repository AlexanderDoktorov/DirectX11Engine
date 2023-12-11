#include "TransformBuffer.h"
#include "Graphics.h"

std::unique_ptr<VertexConstantBuffer<TransformBuffer::Transformations>> TransformBuffer::Cbuff;

TransformBuffer::TransformBuffer(Graphics& Gfx, Drawable& object) : object(object)
{
	if (!Cbuff) Cbuff = std::make_unique<VertexConstantBuffer<Transformations>>(Gfx);
}

void TransformBuffer::Bind(Graphics& Gfx) noexcept
{
	//												Model to World			World to View (rotates world)		View to Projection (flattens objects to fit the camera)
	auto World		= object.GetTransform();
	auto View		= Gfx.GetCamera().GetCameraMatrix();
	auto Projection = Gfx.GetProjection();

	Cbuff->Update(Gfx, Transformations(DirectX::XMMatrixTranspose(World), DirectX::XMMatrixTranspose(View), DirectX::XMMatrixTranspose(Projection)));
	Cbuff->Bind(Gfx);
}