#include "VertexShaderCommon.h"

VertexShaderCommon::VertexShaderCommon(Graphics& Gfx, const wchar_t* FileName) : blob(FileName)
{
	CHECK_HR ( GetDevice(Gfx)->CreateVertexShader(blob.GetBlob()->GetBufferPointer(), blob.GetBlob()->GetBufferSize(), nullptr, &p_VertexShader) );
}

void VertexShaderCommon::LoadFromFile(Graphics& Gfx, const wchar_t* FileName)
{
	blob.ReadFile(FileName);
	CHECK_HR ( GetDevice(Gfx)->CreateVertexShader(blob.GetBlob()->GetBufferPointer(), blob.GetBlob()->GetBufferSize(), nullptr, &p_VertexShader) );
}

void VertexShaderCommon::SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept
{
	GetContext(Gfx)->VSSetShaderResources(start_slot, (UINT)srvs.size(), srvs.data());
}

void VertexShaderCommon::SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept
{
	GetContext(Gfx)->VSSetConstantBuffers(start_slot, (UINT)constant_buffers.size(), constant_buffers.data());
}

void VertexShaderCommon::Unbind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->VSSetShader(nullptr, nullptr, 0U);
}

void VertexShaderCommon::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->VSSetShader(p_VertexShader.Get(), nullptr, 0U);
}

ID3DBlob* VertexShaderCommon::GetBlob() noexcept
{
	return blob.GetBlob();
}

void* VertexShaderCommon::GetShaderImpl() const noexcept
{
	return (void*)p_VertexShader.Get();
}
