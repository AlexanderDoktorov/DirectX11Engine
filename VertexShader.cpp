#include "VertexShader.h"
#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics& Gfx, const wchar_t* fileName)
{
	CHECK_HR(D3DReadFileToBlob(fileName, &p_Blob));
	CHECK_HR(GetDevice(Gfx)->CreateVertexShader(p_Blob->GetBufferPointer(), p_Blob->GetBufferSize(), nullptr, &p_VS));
}

ID3DBlob* VertexShader::GetBlob() noexcept
{
	return p_Blob.Get();
}

void VertexShader::Attach(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->VSSetShader(p_VS.Get(), nullptr, 0U);
}