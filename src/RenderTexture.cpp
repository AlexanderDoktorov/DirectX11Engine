#include "RenderTexture.h"
#include "hrException.h"

RenderTexture::RenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth)
{
    CreateRenderTextureImpl(GetDevice(Gfx), textureFormat, TextureHeight, TextureWidth);
}

void RenderTexture::CreateRenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth)
{
    CreateRenderTextureImpl(GetDevice(Gfx), textureFormat, TextureHeight, TextureWidth);
}

void RenderTexture::CreateRenderTextureImpl(ID3D11Device* pDevice, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth)
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

    HRESULT hr;
    hr = pDevice->CreateTexture2D(&textureDesc, nullptr, &p_Texture); CHECK_HR(hr);

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
    SRVDesc.Format = textureFormat;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0U;
    SRVDesc.Texture2D.MipLevels = ~0U;

    hr = pDevice->CreateShaderResourceView(p_Texture.Get(), &SRVDesc, &p_ShaderResourseView); CHECK_HR(hr);
}

void RenderTexture::Reset()
{
    p_Texture.Reset();
    p_ShaderResourseView.Reset();
}

void RenderTexture::Resize(Graphics& Gfx, UINT TextureHeight, UINT TextureWidth)
{
    // Recreate with new format
    CreateRenderTexture(Gfx, GetDesc().Format, TextureHeight, TextureWidth);
}

