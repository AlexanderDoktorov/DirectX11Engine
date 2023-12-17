#include "WICTexture.h"
#include "Exceptions.h"

#include <Windows.h>
#include <wincodec.h>

WICTexture::WICTexture(Graphics& Gfx, const wchar_t* filePath, UINT bindSlot, wicFlg loadFlags) : TextureBase(bindSlot)
{
    CreateWICTexture(Gfx, filePath, loadFlags);
}

WICTexture::WICTexture(Graphics& Gfx, const char* filePath, UINT bindSlot, wicFlg loadFlags) : TextureBase(bindSlot)
{
    CreateWICTexture(Gfx, filePath, loadFlags);
}

void WICTexture::CreateWICTexture(Graphics& Gfx, const char* filePath, wicFlg loadFlags)
{
    std::string sfilePath(filePath);
    std::wstring wFilePath(sfilePath.begin(), sfilePath.end());

    CreateWICTexture(Gfx, wFilePath.c_str(), loadFlags);
}

void WICTexture::CreateWICTexture(Graphics& Gfx, const wchar_t* filePath, wicFlg loadFlags)
{
    if (loadFlags != DirectX::WIC_LOADER_DEFAULT)
    {
        CHECK_HR ( DirectX::CreateWICTextureFromFileEx(
            GetDevice(Gfx),
            GetContext(Gfx), 
            filePath, 
            0Ui64, 
            D3D11_USAGE_DEFAULT, 
            D3D11_BIND_SHADER_RESOURCE, 
            0, 
            0, 
            loadFlags,
            reinterpret_cast<ID3D11Resource**>(p_Texture.GetAddressOf()), 
            &p_ShaderResourseView
        ));
    }
    else
    {
        CHECK_HR ( DirectX::CreateWICTextureFromFile(
            GetDevice(Gfx), 
            GetContext(Gfx), 
            filePath, 
            reinterpret_cast<ID3D11Resource**>(p_Texture.GetAddressOf()),
            &p_ShaderResourseView) 
        );
    }
}
