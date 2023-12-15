#pragma once
#include "PixelConstantBuffer.h"
#include <memory>
#include <DirectXMath.h>

template<class T, UINT bindSlot>
class DataBufferPS : public IBindable
{
public:
	DataBufferPS(Graphics& Gfx, T& dataRef) : dataRef(dataRef)
	{
		if (!pPixelConstantBuffer)
			pPixelConstantBuffer = std::make_unique<PixelConstantBuffer<T>>(Gfx,bindSlot);
	}

	void Bind(Graphics& Gfx) noexcept override
	{
		pPixelConstantBuffer->Update(Gfx, dataRef);
		pPixelConstantBuffer->Bind(Gfx);
	}

	void Unbind(Graphics& Gfx) noexcept
	{
		ID3D11Buffer* pNullPSDataBuffer[1] = { nullptr };
		GetContext(Gfx)->PSSetConstantBuffers(bindSlot, 1U, pNullPSDataBuffer); // Unbind register(bindSlot)
	}

private:
	static std::unique_ptr<PixelConstantBuffer<T>> pPixelConstantBuffer;
	T& dataRef;
};

template<class T, UINT bindSlot>
std::unique_ptr<PixelConstantBuffer<T>> DataBufferPS<T,bindSlot>::pPixelConstantBuffer {};