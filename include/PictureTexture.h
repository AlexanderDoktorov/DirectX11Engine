#pragma once
#include "GraphicsChild.h"
#include "ITexture.h"
#include <wrl.h>

class PictureTexture : virtual public GraphicsChild, public ITexture
{
public:
	PictureTexture() = default;
	PictureTexture(Graphics& Gfx, const wchar_t* filePath);
	PictureTexture(Graphics& Gfx, const char* filePath); // questinable (casting char* to wchar_t*)

	void CreatePictureTexture(Graphics& Gfx, const char* filePath);
	void CreatePictureTexture(Graphics& Gfx, const wchar_t* filePath);

	ID3D11Texture2D*				GetTexture() const noexcept;
	ID3D11ShaderResourceView*		GetSRV() const noexcept;
	D3D11_TEXTURE2D_DESC			GetDesc() const noexcept;	

	~PictureTexture();

private:
	static bool COMInitilized;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};