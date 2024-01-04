#include "HullShader.h"
#include "hrException.h"
#include <d3dcompiler.h>

HullShader::HullShader(Graphics& Gfx, const wchar_t* FileName)
{
	HRESULT hr;
	hr = D3DReadFileToBlob(FileName, &p_Blob); CHECK_HR(hr);
	hr = GetDevice(Gfx)->CreateHullShader(p_Blob->GetBufferPointer(), p_Blob->GetBufferSize(), nullptr, &p_HullShader); CHECK_HR(hr);
}

void HullShader::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->HSSetShader(p_HullShader.Get(), nullptr, 0U);
}
