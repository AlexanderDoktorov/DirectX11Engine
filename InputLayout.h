#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "IPipelineElement.h"

class InputLayout : public IPipelineElement
{
public:
	InputLayout(Graphics& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& pInputElementDescs, ID3DBlob* ShaderBytecode)
	{
		CHECK_HR ( GetDevice(Gfx)->CreateInputLayout(pInputElementDescs.data(), (UINT)pInputElementDescs.size(), ShaderBytecode->GetBufferPointer(), ShaderBytecode->GetBufferSize(), &p_IL) );
	}

	void Attach(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->IASetInputLayout(p_IL.Get());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> p_IL;
};