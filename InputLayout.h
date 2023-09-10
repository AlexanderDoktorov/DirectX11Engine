#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "DOK_assert.h"
#include "IBindable.h"
#include "IShader.h"

class InputLayout : virtual public IBindable
{
public:
	InputLayout(Graphics& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& pInputElementDescs, IShader* pShader)
	{
		SetLayoutForShader(Gfx, pInputElementDescs, pShader);
	}

	void SetLayoutForShader(Graphics& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& pInputElementDescs, IShader* pShader)
	{
		DOK_assert(pShader->GetBlob() != nullptr, L"Unable to create layout with null shader bytecode");
		CHECK_HR ( GetDevice(Gfx)->CreateInputLayout(pInputElementDescs.data(), (UINT)pInputElementDescs.size(), pShader->GetBlob()->GetBufferPointer(), pShader->GetBlob()->GetBufferSize(), &p_IL));
	}

	void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->IASetInputLayout(p_IL.Get());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> p_IL;
};