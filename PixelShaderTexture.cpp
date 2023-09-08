#include "PixelShaderTexture.h"

void PixelShaderRenderTexture::SetBindSlot(UINT slot) noexcept
{
	bindSlot = slot;
}

UINT PixelShaderRenderTexture::GetBindSlot() const noexcept
{
	return bindSlot;
}

void PixelShaderPictureTexture::SetBindSlot(UINT slot) noexcept
{
	bindSlot = slot;
}

UINT PixelShaderPictureTexture::GetBindSlot() const noexcept
{
	return bindSlot;
}

void PixelShaderRenderTexture::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* srvs[1] = { RenderTexture::GetSRV() };
	GetContext(Gfx)->PSSetShaderResources(bindSlot, 1U, srvs);
}

void PixelShaderPictureTexture::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* srvs[1] = { PictureTexture::GetSRV() };
	GetContext(Gfx)->PSSetShaderResources(bindSlot, 1U, srvs);
}
