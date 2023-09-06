#include "VertexShaderCommon.h"

VertexShaderCommon::VertexShaderCommon(Graphics& Gfx, const wchar_t* FileName) : Shader(FileName)
{
	GetDevice(Gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &p_VertexShader);
}

void VertexShaderCommon::CreateShader(Graphics& Gfx, const wchar_t* FileName)
{
	Shader::ReadFileToBlob(FileName);
	GetDevice(Gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &p_VertexShader);
}

void VertexShaderCommon::CreateInputLayout(Graphics& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& pInputElementDescs)
{
	if (!pInputLayout)
		pInputLayout = std::make_unique<InputLayout>(Gfx, pInputElementDescs, pBlob.Get());
}

void VertexShaderCommon::SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept
{
	GetContext(Gfx)->VSSetShaderResources(start_slot, (UINT)srvs.size(), srvs.data());
}

void VertexShaderCommon::SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept
{
	GetContext(Gfx)->VSSetConstantBuffers(start_slot, (UINT)constant_buffers.size(), constant_buffers.data());
}

void VertexShaderCommon::SetTopology(D3D11_PRIMITIVE_TOPOLOGY tp)
{
	assert(pTopology != nullptr);
	pTopology->SetTopology(tp);
}

void VertexShaderCommon::CreateTopology(D3D11_PRIMITIVE_TOPOLOGY tp)
{
	if (!pTopology)
		pTopology = std::make_unique<Topology>(tp);
}

void VertexShaderCommon::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->VSSetShader(p_VertexShader.Get(), nullptr, 0U);
	if (pInputLayout)	pInputLayout->Bind(Gfx);
	if (pVertexBuffer)	pVertexBuffer->Bind(Gfx);
	if (pTopology)		pTopology->Bind(Gfx);
}
