#include "ComputeShader.h"
#include <d3dcompiler.h>

ComputeShader::ComputeShader(Graphics& Gfx, const wchar_t* fileName)
{
	CHECK_HR(D3DReadFileToBlob(fileName, &p_Blob));
	CHECK_HR(GetDevice(Gfx)->CreateComputeShader(p_Blob->GetBufferPointer(), p_Blob->GetBufferSize(), nullptr, &p_ComputeShader));
}

ID3DBlob* ComputeShader::GetBlob() noexcept
{
	return p_Blob.Get();
}

void ComputeShader::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->CSSetUnorderedAccessViews(0, 1, p_resultUAV.GetAddressOf(), nullptr);
}

void ComputeShader::Unbind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->CSSetUnorderedAccessViews(0, 1, nullptr, nullptr);
}

void ComputeShader::Execute(Graphics& Gfx, UINT num_threads_x, UINT num_threads_y, UINT num_threads_z) noexcept
{
	GetContext(Gfx)->Dispatch(num_threads_x, num_threads_y, num_threads_z);
}