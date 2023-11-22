#pragma once
#include "IShader.h"
#include "Blob.h"
#include "DOK_assert.h"

class VertexShaderCommon : virtual public IShader
{
public:
	VertexShaderCommon(Graphics& Gfx, const wchar_t* FileName);

	void LoadFromFile(Graphics& Gfx, const wchar_t* FileName) override;
	void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept override;
	void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept override;

	virtual void		Bind(Graphics& Gfx) noexcept override;
	ID3DBlob*			GetBlob()			noexcept override;

private:
	void* GetShaderImpl() const noexcept override;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	p_VertexShader;
	Blob blob = Blob();
};