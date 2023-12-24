#include "Blob.h"

Blob::Blob(const wchar_t* ShaderPath)
{
	HRESULT hr = D3DReadFileToBlob(ShaderPath, &pBlob);
	CHECK_HR ( hr );
}

ID3DBlob* Blob::GetBlob() noexcept
{
	return pBlob.Get();
}

void Blob::ReadFile(const wchar_t* ShaderPath)
{
	HRESULT hr = D3DReadFileToBlob(ShaderPath, &pBlob);
	CHECK_HR ( hr );
}
