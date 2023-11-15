#include "TransformBufferPS.h"

std::unique_ptr<PixelConstantBuffer<TransformBufferPS::Transformations>> TransformBufferPS::pPSConstantBuffer{};

TransformBufferPS::TransformBufferPS(Graphics& gfx, Drawable& parent) : parent(parent)
{
	if (!pPSConstantBuffer)
	{
		pPSConstantBuffer = std::make_unique<bufferType>(gfx);
		pPSConstantBuffer->SetBindSlot(1U);
	}
}

void TransformBufferPS::Bind(Graphics & Gfx) noexcept
{
	//												Model to World			World to View (rotates world)		View to Projection (flattens objects to fit the camera)
	auto World		= parent.GetTransform();
	auto View		= Gfx.GetCamera().GetCameraMatrix();
	auto Projection = Gfx.GetProjection();

	pPSConstantBuffer->Update(Gfx, Transformations(dx::XMMatrixTranspose(World), dx::XMMatrixTranspose(View), dx::XMMatrixTranspose(Projection)));
	pPSConstantBuffer->Bind(Gfx);
}

