#pragma once
#include "WICTexture.h"
#include "IBindable.h"
#include "ISlot.h"
#include <DirectXMath.h>
#include <string>
#include "assimp\material.h"

class MaterialTexture : public WICTexture
{
public:
	MaterialTexture() = default;
	MaterialTexture(Graphics& Gfx, const aiTextureType& textureType, const std::string& path, UINT bindSlot = 0U, wicFlg wicLoaderFlags = wicFlg::WIC_FLAGS_NONE);

	void SetFileName(std::string fileName) noexcept;
	void SetFilePath(std::string filePath) noexcept;
	void SetTextureAiType(aiTextureType aiType) noexcept;

	DXGI_FORMAT GetTextureFormat() const noexcept;
	std::string GetFileName() const noexcept;
	std::string GetFilePath() const noexcept;
	const char* GetFilePath_C_str() const noexcept;
	aiTextureType GetTextureAiType() const noexcept;
	bool HasAlphaGloss() const noexcept;
private:
	bool		  hasAlpha				 = false;
	aiTextureType textureType			  = aiTextureType_NONE;
	std::string	  textureFilePath		  = "FILE_PATH_UNKNOWN";
	std::string	  textureFileName		  = "FILE_NAME_UNKNOWN";
};