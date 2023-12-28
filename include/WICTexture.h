#pragma once
#include "TextureBase.h"
#include "DirectXTex.h"
#include <wrl.h>
#include <string>

class WICTexture : public TextureBase
{
public:
	typedef DirectX::WIC_FLAGS wicFlg;

	WICTexture() = default;
	WICTexture(
		Graphics& Gfx, 
		const char* path, 
		UINT bindSlot = 0U, 
		DirectX::WIC_FLAGS loadFlags = DirectX::WIC_FLAGS::WIC_FLAGS_NONE
	);

	static std::string GenerateUID(const char* path, UINT slot);

	bool HasAlphaGloss() const noexcept; 
private:
	bool hasAlphaGloss = false;
	DirectX::ScratchImage image{};
};