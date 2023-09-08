#pragma once
#include "GraphicsChild.h"
#include "ITexture.h"
#include <wrl.h>

class PictureTexture : virtual public GraphicsChild, public ITexture
{
public:
	PictureTexture(Graphics& Gfx, const wchar_t* FileName);

	ID3D11Texture2D*				GetTexture() const noexcept;
	ID3D11ShaderResourceView*		GetSRV() const noexcept;
	D3D11_TEXTURE2D_DESC			GetDesc() const noexcept;	

	~PictureTexture();

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};