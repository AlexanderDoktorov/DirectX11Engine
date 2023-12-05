#include "Material.h"
#include <filesystem>

std::vector<std::shared_ptr<Texture2D>>  Material::loadedTextures{};
int Material::materialsCount = int(0);

Material::Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory) 
	: 
	materialName(pMaterial->GetName().C_Str()), 
	materialDirectory(materialDirectory),
	// Set material index with global materialsCount and increase it
	materialIndex(materialsCount++)
{
	ProcessMaterial(Gfx, pMaterial);

	// Fill mat Desc
	matDesc.Ks = GetPropertyAsColorDx(AI_MATKEY_COLOR_SPECULAR);
	matDesc.Kd = GetPropertyAsColorDx(AI_MATKEY_COLOR_DIFFUSE);
	matDesc.Ka = GetPropertyAsColorDx(AI_MATKEY_COLOR_AMBIENT);
	matDesc.Ns = GetPropertyAs<float>(AI_MATKEY_SHININESS);
	matDesc.hasDiffuseMap  = pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0;
	matDesc.hasNormalMap   = pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0;
	matDesc.hasSpecularMap = pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0;
	matDesc.hasHeightMap   = pMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0;

}

void Material::ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial)
{
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT);
	LoadMaterialProperties(pMaterial);
}

const std::vector<std::shared_ptr<Texture2D>>& Material::GetTextures() const noexcept
{
	return materialTextures;
}

void Material::ShowControlWindow(Graphics& Gfx)
{
	if(ImGui::Begin(("Material control window with indx = " + std::to_string(materialIndex)).c_str()))
	{
		bool changed = false;

		changed |= ImGui::ColorEdit3("Diffuse material color", &matDesc.Kd.x);
		changed |= ImGui::ColorEdit3("Ambient material color", &matDesc.Ka.x);
		changed |= ImGui::ColorEdit3("Specular material color", &matDesc.Ks.x);
		changed |= ImGui::DragFloat("Shininess", &matDesc.Ns);

		if (changed)
			Gfx.UpdateMaterialAt(matDesc, materialIndex);
	}
	ImGui::End();
}

int Material::IsLoaded(const std::string& textureFileName, aiTextureType textureType) noexcept
{
	int i = 0;
	for (const auto& text : loadedTextures)
	{
		if (text->GetTextureAiType() == textureType)
		{
			if (text->GetFileName() == textureFileName)
				return i;
		}
		++i;
	}
	return -1;
}

MaterialDesc Material::GetDesc() const noexcept
{
	return matDesc;
}

int Material::GetIndex() const noexcept
{
	return materialIndex;
}

bool Material::HasNormalMaps() const noexcept
{
	return matDesc.hasNormalMap;
}
bool Material::HasDiffuseMaps() const noexcept
{
	return matDesc.hasDiffuseMap;
}
bool Material::HasSpecularMaps() const noexcept
{
	return matDesc.hasSpecularMap;
}
bool Material::HasHeightMaps() const noexcept
{
	return matDesc.hasHeightMap;
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
	for (size_t i = 0; i < pMaterial->mNumProperties; i++)
	{
		auto pProperty = pMaterial->mProperties[i];
		materialProperties.push_back(std::make_shared<MaterialProperty>(pProperty->mKey.C_Str(), pProperty->mIndex, pProperty->mType, pProperty->mData, pProperty->mDataLength));
	}
}
