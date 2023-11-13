#include "Blob.h"

Blob::Blob(const wchar_t* ShaderPath)
{
	CHECK_HR ( D3DReadFileToBlob(ShaderPath, &pBlob) );
}

ID3DBlob* Blob::GetBlob() noexcept
{
	return pBlob.Get();
}

void Blob::ReadFile(const wchar_t* ShaderPath)
{
	CHECK_HR ( D3DReadFileToBlob(ShaderPath, &pBlob) );
}
