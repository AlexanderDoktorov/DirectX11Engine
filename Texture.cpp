#include "WICTextureLoader.h"
#include "DOK_assert.h"
#include "Texture.h"

Texture::Texture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth)
{
    CreateRTVTexture(Gfx, textureFormat, TextureHeight, TextureWidth);
}

Texture::Texture(Graphics& Gfx, const wchar_t* FileName)
{
	CHECK_HR ( CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) );
	CHECK_HR ( CreateWICTextureFromFile(GetDevice(Gfx), GetContext(Gfx), FileName, reinterpret_cast<ID3D11Resource**>(p_Texture.GetAddressOf()), &p_ShaderResourseView));
}

Texture::~Texture()
{
	CoUninitialize();
}

void Texture::CreateRTVTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = TextureWidth;  // Width of the G-Buffer textures
    textureDesc.Height = TextureHeight; // Height of the G-Buffer textures
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = textureFormat; // Use the appropriate format
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    GetDevice(Gfx)->CreateTexture2D(&textureDesc, nullptr, &p_Texture);

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
    SRVDesc.Format = textureFormat;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0U;
    SRVDesc.Texture2D.MipLevels = ~0U;

    GetDevice(Gfx)->CreateShaderResourceView(p_Texture.Get(), &SRVDesc, &p_ShaderResourseView);
}

void Texture::Reset()
{
    p_Texture.Reset();
    p_ShaderResourseView.Reset();
}

void Texture::Resize(Graphics& Gfx, UINT TextureHeight, UINT TextureWidth)
{
    CreateRTVTexture(Gfx, GetDesc().Format, TextureHeight, TextureWidth);
}

ID3D11Texture2D* Texture::GetTexture() const noexcept
{
    return p_Texture.Get();
}

ID3D11ShaderResourceView* Texture::GetSRV() const noexcept
{
    return p_ShaderResourseView.Get();
}

D3D11_TEXTURE2D_DESC Texture::GetDesc() const noexcept
{
    DOK_assert(p_Texture != nullptr, L"Trying to get desc from empty texture");

    D3D11_TEXTURE2D_DESC desc;
    p_Texture->GetDesc(&desc);
    return desc;
}

void Texture::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShaderResources(3U, 1U, p_ShaderResourseView.GetAddressOf());
}
