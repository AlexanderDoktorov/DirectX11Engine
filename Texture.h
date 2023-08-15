#pragma once
#include "IPipelineElement.h"
#include <wrl.h>

class Texture : public IPipelineElement
{
public:
	Texture(Graphics& Gfx, const wchar_t* FileName);

	~Texture();

	virtual void Attach(Graphics& Gfx) noexcept;

	Microsoft::WRL::ComPtr<ID3D11Resource> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};