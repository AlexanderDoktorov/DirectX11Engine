#pragma once
#include "Drawable.h"
#include "PixelConstantBuffer.h"
#include <DirectXMath.h>

class TransformBufferPS : public IBindable
{
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

	typedef PixelConstantBuffer<Transformations> bufferType;

public:
	TransformBufferPS(Graphics& gfx, Drawable& parent);

	virtual void Bind(Graphics& Gfx) noexcept override;
	
private:
	const Drawable& parent;
	static std::unique_ptr<bufferType> pPSConstantBuffer;
};