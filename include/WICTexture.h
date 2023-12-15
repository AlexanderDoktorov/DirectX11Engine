#pragma once
#include "WICTextureLoader11.h"
#include "TextureBase.h"
#include "IShaderResourse.h"
#include <wrl.h>

class WICTexture : public TextureBase
{
	using wicFlg = DirectX::WIC_LOADER_FLAGS;
public:
	WICTexture() = default;
	WICTexture(Graphics& Gfx, const wchar_t* filePath, UINT bindSlot = 0U, wicFlg loadFlags = wicFlg::WIC_LOADER_DEFAULT);
	WICTexture(Graphics& Gfx, const char* filePath, UINT bindSlot = 0U, wicFlg loadFlags = wicFlg::WIC_LOADER_DEFAULT);

	void CreateWICTexture(Graphics& Gfx, const char* filePath, wicFlg loadFlags = DirectX::WIC_LOADER_DEFAULT);
	void CreateWICTexture(Graphics& Gfx, const wchar_t* filePath, wicFlg loadFlags = DirectX::WIC_LOADER_DEFAULT);
};