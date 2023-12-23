#include "PixelShader.h"
#include "hrException.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& Gfx, const wchar_t* fileName)
{
	HRESULT hr;
	hr = D3DReadFileToBlob(fileName, &p_Blob); CHECK_HR(hr);
	hr = GetDevice(Gfx)->CreatePixelShader(p_Blob->GetBufferPointer(), p_Blob->GetBufferSize(), nullptr, &p_PS); CHECK_HR(hr);
}

ID3DBlob* PixelShader::GetBlob() noexcept
{
	return p_Blob.Get();
}

void PixelShader::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShader(p_PS.Get(), nullptr, 0U);
}