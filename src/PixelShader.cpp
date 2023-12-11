#include "PixelShader.h"
#include "Exceptions.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& Gfx, const wchar_t* fileName)
{
	CHECK_HR(D3DReadFileToBlob(fileName, &p_Blob));
	CHECK_HR(GetDevice(Gfx)->CreatePixelShader(p_Blob->GetBufferPointer(), p_Blob->GetBufferSize(), nullptr, &p_PS));
}

ID3DBlob* PixelShader::GetBlob() noexcept
{
	return p_Blob.Get();
}

void PixelShader::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShader(p_PS.Get(), nullptr, 0U);
}