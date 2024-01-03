#pragma once
#include <TextureBase.h>
#include <DirectXTex.h>

namespace std {
	template<class T>
	class shared_ptr;
}

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

	static std::shared_ptr<WICTexture> Resolve(
		Graphics& Gfx, 
		const char* path,
		UINT bindSlot,
		DirectX::WIC_FLAGS loadFlags);

	static std::string GenerateID(const char* path, UINT slot, DirectX::WIC_FLAGS loadFlags);

	bool AlphaLoaded() const noexcept; 
	DXGI_FORMAT GetFormat() const noexcept;
private:
	DirectX::ScratchImage image{};
};