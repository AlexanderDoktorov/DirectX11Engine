#pragma once
#include "IBindable.h"
#include "Graphics.h"

class HullShader : public IBindable
{
public:
	HullShader(Graphics& Gfx, const wchar_t* FileName);

	virtual void Bind(Graphics& Gfx) noexcept override;

private:
	wrl::ComPtr<ID3D11HullShader> p_HullShader;
	wrl::ComPtr<ID3DBlob> p_Blob;
};