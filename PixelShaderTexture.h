#pragma once
#include "IBindable.h"
#include "Texture.h"

template<UINT startSlot>
class PixelShaderTexture : public Texture, public IBindable
{
public:
	using Texture::Texture;
	
	virtual void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11ShaderResourceView* srvs[1] = { GetSRV() };
		GetContext(Gfx)->PSSetShaderResources(startSlot, 1U, srvs);
	}
};

typedef PixelShaderTexture<0U> TextureSlot0;
typedef PixelShaderTexture<1U> TextureSlot1;
typedef PixelShaderTexture<2U> TextureSlot2;
typedef PixelShaderTexture<3U> TextureSlot3;