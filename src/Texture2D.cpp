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

void Texture2D::SetFileName(std::string fileName) noexcept
{
	textureFileName = std::move(fileName);
}

void Texture2D::SetFilePath(std::string filePath) noexcept
{
	textureFilePath = std::move(filePath);
}

void Texture2D::SetTextureAiType(aiTextureType aiType) noexcept
{
	textureType = aiType;
}

std::string Texture2D::GetFileName() const noexcept
{
	return textureFileName;
}

std::string Texture2D::GetFilePath() const noexcept
{
	return textureFilePath;
}

const char* Texture2D::GetFilePath_C_str() const noexcept
{
	return textureFilePath.c_str();
}

aiTextureType Texture2D::GetTextureAiType() const noexcept
{
	return textureType;
}
