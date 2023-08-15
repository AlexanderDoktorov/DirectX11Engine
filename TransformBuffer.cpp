#include "TransformBuffer.h"

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformBuffer::Cbuff;

TransformBuffer::TransformBuffer(Graphics& Gfx, PipelineObject& parent) : parent(parent)
{
	if (!Cbuff) Cbuff = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(Gfx);
}

void TransformBuffer::Attach(Graphics& Gfx) noexcept
{
	Cbuff->Update(Gfx,       //             World()       View+Projection()
		dx::XMMatrixTranspose(parent.GetTransform() * Gfx.GetProjection())
	);
	Cbuff->Attach(Gfx);
}