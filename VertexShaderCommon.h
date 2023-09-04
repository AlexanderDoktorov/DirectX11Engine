#pragma once
#include "Shader.h"
#include "InputLayout.h"
#include "VertexBuffer.h"
#include "DOK_assert.h"

class VertexShaderCommon : public Shader
{
public:
	VertexShaderCommon(Graphics& Gfx, const wchar_t* FileName);

	void CreateShader(Graphics& Gfx, const wchar_t* FileName);
	void CreateInputLayout(Graphics& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& pInputElementDescs);

	template<class T>
	void CreateVertexBuffer(Graphics& Gfx, const std::vector<T>& verts)
	{
		pVertexBuffer = std::make_unique<VertexBuffer>(Gfx, verts);
	}

	void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept override;
	void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept override;
	void Bind(Graphics& Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	p_VertexShader;
	std::unique_ptr<InputLayout>				pInputLayout;
	std::unique_ptr<VertexBuffer>				pVertexBuffer;
};