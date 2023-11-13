#pragma once
#include <d3d11.h>

struct ITexture
{
	virtual ~ITexture() = default;
	
	virtual ID3D11Texture2D*				GetTexture() const noexcept = 0;
	virtual ID3D11ShaderResourceView*		GetSRV() const noexcept = 0;
	virtual D3D11_TEXTURE2D_DESC			GetDesc() const noexcept = 0;	
};