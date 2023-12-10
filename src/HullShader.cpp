#include "HullShader.h"
#include "Exceptions.h"

HullShader::HullShader(Graphics& Gfx, const wchar_t* FileName)
{
	CHECK_HR ( D3DReadFileToBlob(FileName, &p_Blob) );
	CHECK_HR ( GetDevice(Gfx)->CreateHullShader(p_Blob->GetBufferPointer(), p_Blob->GetBufferSize(), nullptr, &p_HullShader) );
}

void HullShader::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->HSSetShader(p_HullShader.Get(), nullptr, 0U);
}
