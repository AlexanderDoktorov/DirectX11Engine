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

	std::string GetFileName() const noexcept;

	aiTextureType textureType			  = aiTextureType_NONE;
	std::string	  textureFilePath		  = "FILE_PATH_UNKNOWN";
	std::string	  textureFileName		  = "FILE_NAME_UNKNOWN";
};