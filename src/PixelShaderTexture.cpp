#include "PixelShaderTexture.h"

void PixelShaderRenderTexture::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* srvs[1] = { RenderTexture::GetSRV() };
	GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, srvs);
}

void PixelShaderPictureTexture::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* srvs[1] = { PictureTexture::GetSRV() };
	GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, srvs);
}
