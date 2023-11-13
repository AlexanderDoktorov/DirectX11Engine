#include "PixelShaderCommon.h"

PixelShaderCommon::PixelShaderCommon(Graphics& Gfx, const wchar_t* FileName) : blob(FileName)
{
	GetDevice(Gfx)->CreatePixelShader(blob.GetBlob()->GetBufferPointer(), blob.GetBlob()->GetBufferSize(), nullptr, &p_PixelShader);
}

void PixelShaderCommon::Load(Graphics& Gfx, const wchar_t* FileName)
{
	blob.ReadFile(FileName);
	GetDevice(Gfx)->CreatePixelShader(blob.GetBlob()->GetBufferPointer(), blob.GetBlob()->GetBufferSize(), nullptr, &p_PixelShader);
}

void PixelShaderCommon::SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept
{
	GetContext(Gfx)->PSSetShaderResources(start_slot, (UINT)srvs.size(), srvs.data());
}

void PixelShaderCommon::SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept
{
	GetContext(Gfx)->PSSetConstantBuffers(start_slot, (UINT)constant_buffers.size(), constant_buffers.data());
}

void PixelShaderCommon::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShader(p_PixelShader.Get(), nullptr, 0U);
}

ID3D11PixelShader* PixelShaderCommon::GetShader() const noexcept
{
	return p_PixelShader.Get();
}

ID3DBlob* PixelShaderCommon::GetBlob() noexcept
{
	return blob.GetBlob();
}
