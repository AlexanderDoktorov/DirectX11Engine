#pragma once
#include "IBindable.h"
#include <wrl.h>

class Texture : public IBindable
{
public:
	Texture(Graphics& Gfx, const wchar_t* FileName);

	~Texture();

	virtual void Bind(Graphics& Gfx) noexcept;

	Microsoft::WRL::ComPtr<ID3D11Resource> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};