#pragma once
#include "IShader.h"
#include "Blob.h"
#include "Sampler.h"
#include "DOK_assert.h"

class PixelShaderCommon : virtual public IShader
{
public:
	PixelShaderCommon(Graphics& Gfx, const wchar_t* FileName);

	void Load(Graphics& Gfx, const wchar_t* FileName);
	void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept override;
	void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept override;

	virtual void Bind(Graphics& Gfx) noexcept override;
	ID3D11PixelShader* GetShader() const noexcept;

	ID3DBlob*			GetBlob() noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	p_PixelShader;
	Blob blob = Blob();
};