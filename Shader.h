#pragma once
#include "IBindable.h"
#include "ConstantBuffer.h"
#include <d3dcompiler.h>

class Shader : public IBindable
{
public:
	Shader(const wchar_t* ShaderPath)
	{
		ReadFileToBlob(ShaderPath);
	}

	void ReadFileToBlob(const wchar_t* ShaderPath)
	{
		D3DReadFileToBlob(ShaderPath, &pBlob);
	}

	ID3DBlob* GetBlob() noexcept
	{
		return pBlob.Get();
	}

	virtual void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept = 0;
	virtual void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept = 0;

protected:
	wrl::ComPtr<ID3DBlob> pBlob;
};