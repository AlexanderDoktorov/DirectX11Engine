#include "TransformBufferPS.h"
#include "Graphics.h"

std::unique_ptr<PixelConstantBuffer<TransformBufferPS::Transformations>> TransformBufferPS::pPSConstantBuffer{};

TransformBufferPS::TransformBufferPS(Graphics& gfx, Drawable& parent) : parent(parent)
{
	if (!pPSConstantBuffer)
	{
		pPSConstantBuffer = std::make_unique<bufferType>(gfx);
		pPSConstantBuffer->SetShaderBindSlot<shader_type_pixel>(1U);
	}
}

void TransformBufferPS::Bind(Graphics & Gfx) noexcept
{
	//												Model to World			World to View (rotates world)		View to Projection (flattens objects to fit the camera)
	auto World		= parent.GetTransform();
	auto View		= Gfx.GetCamera().GetCameraMatrix();
	auto Projection = Gfx.GetProjection();

	pPSConstantBuffer->Update(Gfx, Transformations(DirectX::XMMatrixTranspose(World), DirectX::XMMatrixTranspose(View), DirectX::XMMatrixTranspose(Projection)));
	pPSConstantBuffer->Bind(Gfx);
}

