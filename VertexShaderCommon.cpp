#include "VertexShaderCommon.h"

VertexShaderCommon::VertexShaderCommon(Graphics& Gfx, const wchar_t* FileName) : blob(FileName)
{
	GetDevice(Gfx)->CreateVertexShader(blob.GetBlob()->GetBufferPointer(), blob.GetBlob()->GetBufferSize(), nullptr, &p_VertexShader);
}

void VertexShaderCommon::Load(Graphics& Gfx, const wchar_t* FileName)
{
	blob.ReadFile(FileName);
	GetDevice(Gfx)->CreateVertexShader(blob.GetBlob()->GetBufferPointer(), blob.GetBlob()->GetBufferSize(), nullptr, &p_VertexShader);
}

void VertexShaderCommon::SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept
{
	GetContext(Gfx)->VSSetShaderResources(start_slot, (UINT)srvs.size(), srvs.data());
}

void VertexShaderCommon::SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept
{
	GetContext(Gfx)->VSSetConstantBuffers(start_slot, (UINT)constant_buffers.size(), constant_buffers.data());
}

void VertexShaderCommon::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->VSSetShader(p_VertexShader.Get(), nullptr, 0U);
}

ID3D11VertexShader* VertexShaderCommon::GetShader() const noexcept
{
	return p_VertexShader.Get();
}

ID3DBlob* VertexShaderCommon::GetBlob() noexcept
{
	return blob.GetBlob();
}
