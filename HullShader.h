#pragma once
#include "IPipelineElement.h"

class HullShader : public IPipelineElement
{
public:
	HullShader(Graphics& Gfx, const wchar_t* FileName);

	virtual void Attach(Graphics& Gfx) noexcept override;

private:
	wrl::ComPtr<ID3D11HullShader> p_HullShader;
	wrl::ComPtr<ID3DBlob> p_Blob;
};