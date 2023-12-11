#include "TextureBase.h"
#include "Graphics.h"

TextureBase::TextureBase()
{}

TextureBase::TextureBase(UINT bindSlot)
	: Slotted(bindSlot)
{}
void TextureBase::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* srvs[] = { GetSRV() };
	GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, srvs);
}

ID3D11Texture2D* TextureBase::GetTexture() const noexcept
{
	return p_Texture.Get();
}

ID3D11ShaderResourceView* TextureBase::GetSRV() const noexcept
{
	return p_ShaderResourseView.Get();
}

D3D11_TEXTURE2D_DESC TextureBase::GetDesc() const noexcept
{
	assert(p_Texture != nullptr && L"Trying to get desc from null RenderTexture");

	D3D11_TEXTURE2D_DESC desc{};
	if(p_Texture)
		p_Texture->GetDesc(&desc);

	return desc;
}
