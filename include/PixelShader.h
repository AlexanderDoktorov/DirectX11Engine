#pragma once
#include <wrl.h>
#include "IBindable.h"

class PixelShader : public IBindable
{
public:
	PixelShader(Graphics& Gfx, const wchar_t* fileName);

	ID3DBlob* GetBlob() noexcept;

	void Bind(Graphics& Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> p_PS;
	Microsoft::WRL::ComPtr<ID3DBlob> p_Blob;
};