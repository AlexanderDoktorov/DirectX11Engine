#include "WICTextureLoader.h"
#include "PictureTexture.h"
#include "Exceptions.h"

PictureTexture::PictureTexture(Graphics& Gfx, const wchar_t* FileName)
{
	CHECK_HR ( CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) );
	CHECK_HR ( CreateWICTextureFromFile(GetDevice(Gfx), GetContext(Gfx), FileName, reinterpret_cast<ID3D11Resource**>(p_Texture.GetAddressOf()), &p_ShaderResourseView));
}

PictureTexture::~PictureTexture()
{
	CoUninitialize();
}

ID3D11Texture2D* PictureTexture::GetTexture() const noexcept
{
    return p_Texture.Get();
}

ID3D11ShaderResourceView* PictureTexture::GetSRV() const noexcept
{
    return p_ShaderResourseView.Get();
}

D3D11_TEXTURE2D_DESC PictureTexture::GetDesc() const noexcept
{
    DOK_assert(p_Texture != nullptr, L"Trying to get desc from empty texture");

    D3D11_TEXTURE2D_DESC desc;
    p_Texture->GetDesc(&desc);
    return desc;
}
