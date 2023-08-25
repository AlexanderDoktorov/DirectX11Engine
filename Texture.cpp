#include "WICTextureLoader.h"
#include "DOK_assert.h"
#include "Texture.h"

Texture::Texture(Graphics& Gfx, const wchar_t* FileName)
{
	CHECK_HR ( CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) );
	CHECK_HR ( CreateWICTextureFromFile(GetDevice(Gfx), GetContext(Gfx), FileName, &p_Texture, &p_ShaderResourseView) );
}

Texture::~Texture()
{
	CoUninitialize();
}

void Texture::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShaderResources(0U, 1U, p_ShaderResourseView.GetAddressOf());
	GetContext(Gfx)->VSSetShaderResources(0U, 1U, p_ShaderResourseView.GetAddressOf());
}
