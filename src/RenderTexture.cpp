#include "RenderTexture.h"
#include "Exceptions.h"

RenderTexture::RenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth)
{
    CreateRenderTexture(Gfx, textureFormat, TextureHeight, TextureWidth);
}

RenderTexture::RenderTexture(Graphics& Gfx, const RenderTexture& otherTexture)
{
    CreateRenderTexture(Gfx, otherTexture.GetDesc().Format, otherTexture.GetDesc().Height, otherTexture.GetDesc().Width);
}

void RenderTexture::CreateRenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth)
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

    CHECK_HR( GetDevice(Gfx)->CreateTexture2D(&textureDesc, nullptr, &p_Texture) );

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
    SRVDesc.Format = textureFormat;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0U;
    SRVDesc.Texture2D.MipLevels = ~0U;

    CHECK_HR ( GetDevice(Gfx)->CreateShaderResourceView(p_Texture.Get(), &SRVDesc, &p_ShaderResourseView) );
}

void RenderTexture::Reset()
{
    p_Texture.Reset();
    p_ShaderResourseView.Reset();
}

void RenderTexture::Resize(Graphics& Gfx, UINT TextureHeight, UINT TextureWidth)
{
    CreateRenderTexture(Gfx, GetDesc().Format, TextureHeight, TextureWidth);
}

ID3D11Texture2D* RenderTexture::GetTexture() const noexcept
{
    return p_Texture.Get();
}

ID3D11ShaderResourceView* RenderTexture::GetSRV() const noexcept
{
    return p_ShaderResourseView.Get();
}

D3D11_TEXTURE2D_DESC RenderTexture::GetDesc() const noexcept
{
    DOK_assert(p_Texture != nullptr, L"Render texture was null");

    D3D11_TEXTURE2D_DESC desc{};
    p_Texture->GetDesc(&desc);

    return desc;
}

