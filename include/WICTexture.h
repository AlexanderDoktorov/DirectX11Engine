#pragma once
#include "TextureBase.h"
#include "DirectXTex.h"
#include <wrl.h>
#include <string>
#include <future>

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

	static std::future<std::shared_ptr<WICTexture>> LoadTextureAsync(
		Graphics& Gfx, 
		const char* filePath, 
		UINT bindSlot = 0U, 
		DirectX::WIC_FLAGS loadFlags = DirectX::WIC_FLAGS::WIC_FLAGS_NONE
	) 
	{
		return std::async(std::launch::async, [&]() {
			return WICTexture::Resolve(Gfx, filePath, bindSlot, loadFlags);
			});
	}

	static std::shared_ptr<WICTexture> Resolve(
		Graphics& Gfx, 
		const char* path,
		UINT bindSlot,
		DirectX::WIC_FLAGS loadFlags);

	static std::string GenerateID(const char* path, UINT slot, DirectX::WIC_FLAGS loadFlags);

	bool HasAlphaGloss() const noexcept; 
	DXGI_FORMAT GetFormat() const noexcept;
private:
	bool hasAlphaGloss = false;
	DirectX::ScratchImage image{};
};