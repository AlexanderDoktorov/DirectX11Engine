#include "WICTextureLoader11.h"
#include "PictureTexture.h"
#include "Exceptions.h"

bool PictureTexture::COMInitilized = false;

PictureTexture::PictureTexture(Graphics& Gfx, const wchar_t* filePath)
{
    CreatePictureTexture(Gfx, filePath);
}

PictureTexture::PictureTexture(Graphics& Gfx, const char* filePath)
{
    CreatePictureTexture(Gfx, filePath);
}

void PictureTexture::CreatePictureTexture(Graphics& Gfx, const char* filePath)
{
    std::string sfilePath(filePath);
    std::wstring wFilePath(sfilePath.begin(), sfilePath.end());

    if (!COMInitilized)
    {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); CHECK_HR(hr);
        COMInitilized = SUCCEEDED(hr);
    }
    CHECK_HR ( DirectX::CreateWICTextureFromFile(GetDevice(Gfx), GetContext(Gfx), wFilePath.c_str(), reinterpret_cast<ID3D11Resource**>(p_Texture.GetAddressOf()), &p_ShaderResourseView));
}

void PictureTexture::CreatePictureTexture(Graphics& Gfx, const wchar_t* filePath)
{
    if (!COMInitilized)
    {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); CHECK_HR(hr);
        COMInitilized = SUCCEEDED(hr);
    }
    CHECK_HR ( DirectX::CreateWICTextureFromFile(GetDevice(Gfx), GetContext(Gfx), filePath, reinterpret_cast<ID3D11Resource**>(p_Texture.GetAddressOf()), &p_ShaderResourseView) );
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
