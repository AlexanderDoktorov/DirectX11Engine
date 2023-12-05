#pragma once
#include "PictureTexture.h"
#include <DirectXMath.h>
#include <string>
#include "assimp\material.h"

class Texture2D : public PictureTexture
{
public:
	Texture2D() = default;
	Texture2D(Graphics& Gfx, const aiTextureType& textureType, const std::string& path);

	void SetFileName(std::string fileName) noexcept;
	void SetFilePath(std::string filePath) noexcept;
	void SetTextureAiType(aiTextureType aiType) noexcept;

	std::string GetFileName() const noexcept;
	std::string GetFilePath() const noexcept;
	const char* GetFilePath_C_str() const noexcept;
	aiTextureType GetTextureAiType() const noexcept;
private:
	aiTextureType textureType			  = aiTextureType_NONE;
	std::string	  textureFilePath		  = "FILE_PATH_UNKNOWN";
	std::string	  textureFileName		  = "FILE_NAME_UNKNOWN";
};