#include "ShaderResoursesPS.h"

void RTTexturePS::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* srvs[1] = { RenderTexture::GetSRV() };
	GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, srvs);
}

void TextureArrayPS::Bind(Graphics& Gfx) noexcept
{
	std::vector<ID3D11ShaderResourceView*> srvs;
	for (size_t i = 0; i < pictures.size(); i++)
	{
		srvs.push_back(pictures[i].GetSRV());
	}
	GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, srvs.data());
}
