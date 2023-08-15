#pragma once
#include "IPipelineElement.h"

class PixelShader : public IPipelineElement
{
public:
	PixelShader(Graphics& Gfx, const wchar_t* fileName);

	ID3DBlob* GetBlob() noexcept;

	void Attach(Graphics& Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> p_PS;
	Microsoft::WRL::ComPtr<ID3DBlob> p_Blob;
};