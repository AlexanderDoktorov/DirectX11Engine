#pragma once
#include "WICTextureLoader11.h"
#include "TextureBase.h"
#include "IShaderResourse.h"
#include "DirectXTex.h"
#include <wrl.h>

class WICTexture : public TextureBase
{
public:
	typedef DirectX::WIC_FLAGS wicFlg;

	WICTexture() = default;
	WICTexture(Graphics& Gfx, const wchar_t* filePath, UINT bindSlot = 0U, wicFlg loadFlags = wicFlg::WIC_FLAGS_NONE);
	WICTexture(Graphics& Gfx, const char* filePath, UINT bindSlot = 0U, wicFlg loadFlags = wicFlg::WIC_FLAGS_NONE);

	HRESULT CreateWICTexture(Graphics& Gfx, const char* filePath, wicFlg loadFlags = wicFlg::WIC_FLAGS_NONE, bool* hasAlphaGloss = nullptr) noexcept;
	HRESULT CreateWICTexture(Graphics& Gfx, const wchar_t* filePath, wicFlg loadFlags = wicFlg::WIC_FLAGS_NONE, bool* hasAlphaGloss = nullptr) noexcept;
};