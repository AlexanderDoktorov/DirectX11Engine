#pragma once
#include "IBindable.h"
#include <d3dcompiler.h>

class VertexShader : public IBindable
{
public:
	VertexShader(Graphics& Gfx, const wchar_t* fileName)
	{
		CHECK_HR(D3DReadFileToBlob(fileName, &p_Blob));
		CHECK_HR(GetDevice(Gfx)->CreateVertexShader(p_Blob->GetBufferPointer(), p_Blob->GetBufferSize(), nullptr, &p_VS));
	}

	ID3DBlob* GetBlob() noexcept
	{
		return p_Blob.Get();
	}

	void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->VSSetShader(p_VS.Get(), nullptr, 0U);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> p_VS;
	Microsoft::WRL::ComPtr<ID3DBlob> p_Blob;
};
