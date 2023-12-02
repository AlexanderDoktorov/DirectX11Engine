#include "Texture2D.h"
#include <filesystem>

Texture2D::Texture2D(Graphics& Gfx, const aiTextureType& textureType, const std::string& textureFilePath)
	: 
	PictureTexture(Gfx, textureFilePath.c_str()), 
	textureType(textureType), 
	textureFilePath(textureFilePath)
{
	textureFileName = std::filesystem::path(textureFilePath).filename().string();
}

std::string Texture2D::GetFileName() const noexcept
{
	return textureFileName;
}
