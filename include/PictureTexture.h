#pragma once
#include "GraphicsChild.h"
#include "WICTextureLoader11.h"
#include "ITexture.h"
#include <wrl.h>

class PictureTexture : virtual public GraphicsChild, public ITexture
{
public:
	PictureTexture();
	PictureTexture(Graphics& Gfx, const wchar_t* filePath, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT);
	PictureTexture(Graphics& Gfx, const char* filePath, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT);

	void CreatePictureTexture(Graphics& Gfx, const char* filePath, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT);
	void CreatePictureTexture(Graphics& Gfx, const wchar_t* filePath, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT);

	ID3D11Texture2D*				GetTexture() const noexcept;
	ID3D11ShaderResourceView*		GetSRV() const noexcept;
	D3D11_TEXTURE2D_DESC			GetDesc() const noexcept;	

	~PictureTexture();

private:
	static bool COMInitilized;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};