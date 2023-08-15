#pragma once
#include "IPipelineElement.h"

class VertexShader : public IPipelineElement
{
public:
	VertexShader(Graphics& Gfx, const wchar_t* fileName);

	ID3DBlob* GetBlob() noexcept;

	void Attach(Graphics& Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> p_VS;
	Microsoft::WRL::ComPtr<ID3DBlob> p_Blob;
};