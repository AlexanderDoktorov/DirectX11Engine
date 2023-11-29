#include "PictureTexture.h"
#include "Exceptions.h"

bool PictureTexture::COMInitilized = false;

PictureTexture::PictureTexture()
{
    if (!COMInitilized)
    {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); CHECK_HR(hr);
        COMInitilized = SUCCEEDED(hr);
    }
}

PictureTexture::PictureTexture(Graphics& Gfx, const wchar_t* filePath, DirectX::WIC_LOADER_FLAGS loadFlags) : PictureTexture()
{
    CreatePictureTexture(Gfx, filePath, loadFlags);
}

PictureTexture::PictureTexture(Graphics& Gfx, const char* filePath, DirectX::WIC_LOADER_FLAGS loadFlags) : PictureTexture()
{
    CreatePictureTexture(Gfx, filePath, loadFlags);
}

void PictureTexture::CreatePictureTexture(Graphics& Gfx, const char* filePath, DirectX::WIC_LOADER_FLAGS loadFlags)
{
    std::string sfilePath(filePath);
    std::wstring wFilePath(sfilePath.begin(), sfilePath.end());

    CreatePictureTexture(Gfx, wFilePath.c_str(), loadFlags);
}

void PictureTexture::CreatePictureTexture(Graphics& Gfx, const wchar_t* filePath, DirectX::WIC_LOADER_FLAGS loadFlags)
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
