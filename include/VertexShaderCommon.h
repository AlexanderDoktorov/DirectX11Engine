#pragma once
#include "IShader.h"
#include "Blob.h"
#include "DOK_assert.h"

class VertexShaderCommon : virtual public IShader
{
public:
	VertexShaderCommon(Graphics& Gfx, const wchar_t* FileName);

	void Load(Graphics& Gfx, const wchar_t* FileName);
	void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept override;
	void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept override;

	virtual void Bind(Graphics& Gfx) noexcept override;
	ID3D11VertexShader* GetShader() const noexcept;

	ID3DBlob*			GetBlob() noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	p_VertexShader;
	Blob blob = Blob();
};