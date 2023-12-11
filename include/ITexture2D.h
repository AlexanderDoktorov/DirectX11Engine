#pragma once
#include "IShaderResourse.h"

struct ITexture2D : public IShaderResourse
{
	virtual ~ITexture2D() = default;
	
	virtual ID3D11Texture2D*				GetTexture() const noexcept = 0;
	virtual D3D11_TEXTURE2D_DESC			GetDesc() const noexcept = 0;	
};