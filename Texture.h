#pragma once
#include "IBindable.h"
#include <wrl.h>

class Texture
{
public:
	Texture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth);
	Texture(Graphics& Gfx, const wchar_t* FileName);

	void							CreateRTVTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth);
	void							Reset();
	void							Resize(Graphics& Gfx, UINT TextureHeight, UINT TextureWidth);
	ID3D11Texture2D*				GetTexture() const noexcept;
	ID3D11ShaderResourceView*		GetSRV() const noexcept;
	D3D11_TEXTURE2D_DESC			GetDesc() const noexcept;	

	~Texture();

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};