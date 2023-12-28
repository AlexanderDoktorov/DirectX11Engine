#include <Windows.h>
#include "DOK_DX11.h"
#include "WICTexture.h"
#include "hrException.h"

WICTexture::WICTexture(Graphics& Gfx, const char* filePath, UINT bindSlot, DirectX::WIC_FLAGS loadFlags) 
    : 
    TextureBase(bindSlot)
{
    HRESULT hr = LoadFromWICFile(to_wstring(filePath).c_str(), loadFlags, nullptr, image); CHECK_HR(hr);

    // Generate mip maps
    DirectX::ScratchImage mipChain;
    hr = DirectX::GenerateMipMaps(
        image.GetImages(), 
        image.GetImageCount(), 
        image.GetMetadata(), 
        DirectX::TEX_FILTER_FORCE_NON_WIC, 
        /* all levels? */ 0ull, 
        mipChain
    ); CHECK_HR(hr);

    hasAlphaGloss = image.IsAlphaAllOpaque();

    hr = CreateShaderResourceView(
        GetDevice(Gfx), 
        mipChain.GetImages(), 
        mipChain.GetImageCount(), 
        mipChain.GetMetadata(), 
        &p_ShaderResourseView
    ); CHECK_HR(hr);
}

std::string WICTexture::GenerateUID(const char* path, UINT slot)
{
    using namespace std::string_literals;
    return typeid(WICTexture).name() + "#"s + path + "#" + std::to_string( slot );
}

bool WICTexture::HasAlphaGloss() const noexcept
{
    return hasAlphaGloss;
}
