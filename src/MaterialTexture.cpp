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
	textureFileName(std::filesystem::path(path).filename().string())
{
	WICTexture::CreateWICTexture(Gfx, path.c_str(), wicLoadFlags, &hasAlpha);
	WICTexture::SetBindSlot(bindSlot);
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

DXGI_FORMAT MaterialTexture::GetTextureFormat() const noexcept
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	p_ShaderResourseView->GetDesc(&desc);
	return desc.Format;
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

bool MaterialTexture::HasAlphaGloss() const noexcept
{
	return hasAlpha;
}

