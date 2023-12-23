#include "PixelShaderCommon.h"
#include <comdef.h> 

PixelShaderCommon::PixelShaderCommon(Graphics& Gfx, const wchar_t* FileName) : blob(FileName)
{
	HRESULT hr = GetDevice(Gfx)->CreatePixelShader(
		blob.GetBlob()->GetBufferPointer(), 
		blob.GetBlob()->GetBufferSize(), 
		nullptr, 
		&p_PixelShader); 
	CHECK_HR(hr);
}
void PixelShaderCommon::LoadFromFile(Graphics& Gfx, const wchar_t* FileName)
{
	blob.ReadFile(FileName);
	HRESULT hr = GetDevice(Gfx)->CreatePixelShader(
		blob.GetBlob()->GetBufferPointer(), 
		blob.GetBlob()->GetBufferSize(),
		nullptr, 
		&p_PixelShader
	); 
	CHECK_HR(hr);
}
void PixelShaderCommon::SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept
{
	GetContext(Gfx)->PSSetShaderResources(start_slot, (UINT)srvs.size(), srvs.data());
}
void PixelShaderCommon::SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept
{
	GetContext(Gfx)->PSSetConstantBuffers(start_slot, (UINT)constant_buffers.size(), constant_buffers.data());
}
std::shared_ptr<PixelShaderCommon> PixelShaderCommon::Resolve(Graphics& Gfx, const wchar_t* FileName) noexcept
{
	return BindableSystem::Resolve<PixelShaderCommon>(Gfx, FileName);
}
std::string PixelShaderCommon::GenerateID(const wchar_t* FileName) noexcept
{
	_bstr_t asciString(FileName);
	return std::string(typeid(PixelShaderCommon).name()).append("%").append(asciString);
}
void PixelShaderCommon::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShader(p_PixelShader.Get(), nullptr, 0U);
}

void PixelShaderCommon::Unbind(Graphics& Gfx) noexcept
{
	// Unbind pixel shader
	GetContext(Gfx)->PSSetShader(nullptr, nullptr, 0U);
}

ID3DBlob* PixelShaderCommon::GetBlob() noexcept
{
	return blob.GetBlob();
}

void* PixelShaderCommon::GetShaderImpl() const noexcept
{
	return (void*)p_PixelShader.Get();
}
