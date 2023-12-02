#include "Material.h"
#include <filesystem>

std::vector<std::shared_ptr<Texture2D>>  Material::loadedTextures{};

Material::Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory) 
	: 
	materialName(pMaterial->GetName().C_Str()), 
	materialDirectory(materialDirectory)
{
	ProcessMaterial(Gfx, pMaterial);
}

void Material::ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial)
{
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT);
	LoadMaterialProperties(pMaterial);
}

std::shared_ptr<Texture2D> Material::GetTexture(aiTextureType textureType) noexcept
{
	// TO DO
	return {};
}

int Material::IsLoaded(const std::string& textureFileName, aiTextureType textureType) noexcept
{
	int i = 0;
	for (const auto& text : loadedTextures)
	{
		if (text->textureType == textureType)
		{
			if (text->textureFileName == textureFileName)
				return i;
		}
		++i;
	}
	return -1;
}

std::shared_ptr<Texture2D> Material::PushTexture(Graphics& Gfx, const std::string& textureFilePath, aiTextureType textureType) noexcept
{
	std::string textureFileName = std::filesystem::path(textureFilePath).filename().string();
	int indx = IsLoaded(textureFileName, textureType);
	if (indx >= 0)
		return loadedTextures[indx];
	else
	{
		std::shared_ptr<Texture2D> pTexure = std::make_shared<Texture2D>(Gfx, textureType, textureFilePath);
		loadedTextures.push_back(pTexure);
		return pTexure;
	}
}

void Material::LoadMaterialTextures(Graphics& Gfx, aiMaterial* pMaterial, aiTextureType textureType)
{
	for (unsigned int i = 0u; i < pMaterial->GetTextureCount(textureType); i++)
	{
		aiString textureFileName;
		if (pMaterial->GetTexture(textureType, i, &textureFileName) == aiReturn_SUCCESS)
		{
			const std::string textureFilePath =  materialDirectory + '\\' + std::string(textureFileName.C_Str());
			auto pTexture = PushTexture(Gfx, textureFilePath, textureType);
			materialTextures.push_back(pTexture);
		}
	}
}

void Material::LoadMaterialProperties(aiMaterial* pMaterial)
{
	materialProperties.reserve(pMaterial->mNumProperties);
	if (auto pDiffuse = MaterialProperty::Make<EMATERIAL_PROPERTY_COLOR_DIFFUSE>(pMaterial))
	{
		materialProperties.push_back(pDiffuse);
	}
	if (auto pAmbient = MaterialProperty::Make<EMATERIAL_PROPERTY_COLOR_AMBIENT>(pMaterial))
	{
		materialProperties.push_back(pAmbient);
	}
	if (auto pSpecular = MaterialProperty::Make<EMATERIAL_PROPERTY_COLOR_SPECULAR>(pMaterial))
	{
		materialProperties.push_back(pSpecular);
	}
}
