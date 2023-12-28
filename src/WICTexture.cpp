#include <Windows.h>
#include "WICTexture.h"
#include "hrException.h"

WICTexture::WICTexture(Graphics& Gfx, const wchar_t* filePath, UINT bindSlot, wicFlg loadFlags) : TextureBase(bindSlot)
{
    HRESULT hr = CreateWICTexture(Gfx, filePath, loadFlags); CHECK_HR(hr);
}

WICTexture::WICTexture(Graphics& Gfx, const char* filePath, UINT bindSlot, wicFlg loadFlags) : TextureBase(bindSlot)
{
    HRESULT hr = CreateWICTexture(Gfx, filePath, loadFlags); CHECK_HR(hr);
}

HRESULT WICTexture::CreateWICTexture(Graphics& Gfx, const char* filePath, wicFlg loadFlags, bool* hasAlphaGloss) noexcept
{
    std::string sfilePath(filePath);
    std::wstring wFilePath(sfilePath.begin(), sfilePath.end());

    return CreateWICTexture(Gfx, wFilePath.c_str(), loadFlags, hasAlphaGloss);
}

HRESULT WICTexture::CreateWICTexture(Graphics& Gfx, const wchar_t* filePath, wicFlg loadFlags, bool* hasAlphaGloss) noexcept
{
    DirectX::ScratchImage image;
    HRESULT hr = LoadFromWICFile(filePath, loadFlags, nullptr, image);
    if (FAILED(hr))
        return hr;

    // Generate mip maps
    DirectX::ScratchImage mipChain;
    hr = DirectX::GenerateMipMaps(
        image.GetImages(), 
        image.GetImageCount(), 
        image.GetMetadata(), 
        DirectX::TEX_FILTER_FORCE_NON_WIC, 
        /* all levels? */ 0ull, 
        mipChain
    );
    if (FAILED(hr))
        return hr;

    //hr = DirectX::CreateTexture(GetDevice(Gfx), mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), reinterpret_cast<ID3D11Resource**>(p_Texture.GetAddressOf()));
    //if (FAILED(hr))
    //    return hr;

    hr = CreateShaderResourceView(GetDevice(Gfx), mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), &p_ShaderResourseView);

    const DirectX::Image* img = image.GetImage(0, 0, 0);
    if (hasAlphaGloss)
    {
        for (size_t i = 0; i < img->height; ++i)
        {
            for (size_t j = 0; j < img->width; ++j)
            {
                uint8_t* pixel = &(img->pixels[i * img->rowPitch + j * 4]);
                const uint8_t& alpha = pixel[3];
                if (alpha != 255)
                {
                    *hasAlphaGloss = true;
                    return hr;
                }
            }
        }
    }
    return hr;
}
