#include "Material.h"
#include <filesystem>

std::vector<std::shared_ptr<Texture2D>>  Material::loadedTextures{};

Material::Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory, size_t materialIndex) 
	: 
	materialName(pMaterial->GetName().C_Str()), 
	materialDirectory(materialDirectory),
	materialIndex(materialIndex)
{
	ProcessMaterial(Gfx, pMaterial);

	// Fill mat Desc
	matDesc.Ks = reinterpret_cast<dx::XMFLOAT3&&>(GetPropertyAs<aiColor3D>(AI_MATKEY_COLOR_SPECULAR).value());
	matDesc.Kd = reinterpret_cast<dx::XMFLOAT3&&>(GetPropertyAs<aiColor3D>(AI_MATKEY_COLOR_DIFFUSE).value());
	matDesc.Ka = reinterpret_cast<dx::XMFLOAT3&&>(GetPropertyAs<aiColor3D>(AI_MATKEY_COLOR_AMBIENT).value());
	matDesc.Ns = GetPropertyAs<float>(AI_MATKEY_SHININESS).value_or(10.f);
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

void Material::ShowMaterialControls(Graphics& Gfx)
{
	bool changed = false;

	ImGui::Text("Material indx = %d", materialIndex);
	auto makeStr = [=](const char* text) -> std::string
	{
		return text + std::string("##") + std::to_string(materialIndex);
	};
	changed |= ImGui::ColorEdit3( makeStr("Diffuse material color").c_str(), &matDesc.Kd.x);
	changed |= ImGui::ColorEdit3( makeStr("Ambient material color").c_str(), &matDesc.Ka.x);
	changed |= ImGui::ColorEdit3( makeStr("Specular material color").c_str(), &matDesc.Ks.x);
	if(!matDesc.hasSpecularMap)
		changed |= ImGui::DragFloat( makeStr("Shininess").c_str(), &matDesc.Ns);

	if (changed)
		Gfx.UpdateMaterialAt(matDesc, materialIndex);
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
std::string Material::GetName() const noexcept
{
	return materialName;
}
std::string Material::GetDirectory() const noexcept
{
	return materialDirectory;
}
size_t Material::GetIndex() const noexcept
{
	return materialIndex;
}
bool Material::operator==(const Material& rhs) const noexcept
{
	return materialName == rhs.materialName && materialDirectory == rhs.materialDirectory;
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
