#include "MaterialTexture.h"
#include <filesystem>

MaterialTexture::MaterialTexture
(
	Graphics& Gfx, 
	const aiTextureType& textureType, 
	const std::string& path, 
	UINT bindSlot, 
	wicFlg wicLoadFlags
) :
	textureFilePath(path),
	textureType(textureType),
	textureFileName(std::filesystem::path(path).filename().string()),
	WICTexture(Gfx, path.c_str(), bindSlot, wicLoadFlags)
{

}

void MaterialTexture::SetFileName(std::string fileName) noexcept
{
	textureFileName = std::move(fileName);
}

void MaterialTexture::SetFilePath(std::string filePath) noexcept
{
	textureFilePath = std::move(filePath);
}

void MaterialTexture::SetTextureAiType(aiTextureType aiType) noexcept
{
	textureType = aiType;
}

std::string MaterialTexture::GetFileName() const noexcept
{
	return textureFileName;
}

std::string MaterialTexture::GetFilePath() const noexcept
{
	return textureFilePath;
}

const char* MaterialTexture::GetFilePath_C_str() const noexcept
{
	return textureFilePath.c_str();
}

aiTextureType MaterialTexture::GetTextureAiType() const noexcept
{
	return textureType;
}

