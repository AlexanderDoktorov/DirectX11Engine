#include "Material.h"
#include "SlotLayout.h"
#include <filesystem>
#include "DOK_assimp.h"

std::vector<std::shared_ptr<MaterialTexture>>  Material::loadedTextures{};

Material::Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory) 
	: 
	materialName(pMaterial->GetName().C_Str()), 
	materialDirectory(materialDirectory)
{
	ProcessMaterial(Gfx, pMaterial);
}

void Material::ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial)
{
	mapLayout.hasDiffuseMap  = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE,		SLOT_TEXTURE_DIFFUSE);
	mapLayout.hasNormalMap   = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS,		SLOT_TEXTURE_NORMALMAP, wicFlg::WIC_LOADER_IGNORE_SRGB);
	mapLayout.hasSpecularMap = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR,		SLOT_TEXTURE_SPECULAR);
	mapLayout.hasHeightMap   = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT,		SLOT_TEXTURE_HEIGHT);
	LoadMaterialProperties(pMaterial);
}

const std::vector<std::shared_ptr<MaterialTexture>>& Material::GetTextures() const noexcept
{
	return materialTextures;
}

bool Material::ShowMaterialGUI(bool* p_open)
{
	static constexpr ImVec4 red = {1.f,0.f,0.f,1.f};
	static constexpr ImVec4 yellow = {1.f,1.f,0.f,1.f};
	bool changed = matProps.ShowGUI(materialName.c_str());
	ImGui::TextColored(mapLayout.hasDiffuseMap ? yellow : red, "Diffuse map");
	ImGui::TextColored(mapLayout.hasHeightMap ? yellow : red, "Height map");
	ImGui::TextColored(mapLayout.hasNormalMap ? yellow : red, "Normal map");
	ImGui::TextColored(mapLayout.hasSpecularMap ? yellow : red, "Specular map");
	return changed;
}

void Material::Bind(Graphics& Gfx) noexcept
{
	for (const auto& text : materialTextures)
	{
		text->Bind(Gfx);
	}
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

MapLayout Material::GetMapLayout() const noexcept
{
	return mapLayout;
}

bool Material::HasAnyMaps() const noexcept
{
	return mapLayout.hasDiffuseMap || mapLayout.hasHeightMap || mapLayout.hasNormalMap || mapLayout.hasSpecularMap;
}

std::string Material::GetName() const noexcept
{
	return materialName;
}
std::string Material::GetDirectory() const noexcept
{
	return materialDirectory;
}
MaterialPropertiesDesc Material::GetPropertiesDesc() const noexcept
{
	return matProps;
}
MaterialDesc Material::GetMaterialDesc() const noexcept
{
	MaterialDesc materialDesc_{};
	materialDesc_.hasDiffuseMap  = mapLayout.hasDiffuseMap;
	materialDesc_.hasHeightMap	 = mapLayout.hasHeightMap;
	materialDesc_.hasNormalMap	 = mapLayout.hasNormalMap;
	materialDesc_.hasSpecularMap = mapLayout.hasSpecularMap;
	materialDesc_.Ka = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_AMBIENT).value();
	materialDesc_.Kd = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_DIFFUSE).value();
	materialDesc_.Ks = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_SPECULAR).value();
	materialDesc_.Ke = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_SPECULAR).value();
	materialDesc_.Ns = matProps.GetPropertyAs<float>(AI_MATKEY_SHININESS).value();
	materialDesc_.illum = *(int*)matProps.GetProperty(AI_MATKEY_SHADING_MODEL)->GetData().data();
	return materialDesc_;
}
bool Material::operator==(const Material& rhs) const noexcept
{
	return materialName == rhs.materialName && materialDirectory == rhs.materialDirectory;
}

bool Material::LoadMaterialTextures
(
	Graphics& Gfx, 
	aiMaterial* pMaterial, 
	aiTextureType textureType, 
	UINT bindSlot, 
	wicFlg wicFlags
)
{
	bool hasSuchMap = false;
	for (unsigned int i = 0u; i < pMaterial->GetTextureCount(textureType); i++)
	{
		hasSuchMap = true;
		aiString textureFileName;
		if (pMaterial->GetTexture(textureType, i, &textureFileName) == aiReturn_SUCCESS)
		{
			const std::string textureFilePath =  materialDirectory + '\\' + std::string(textureFileName.C_Str());
			auto pTexture = PushTexture(Gfx, textureFilePath, textureType, bindSlot, wicFlags);
			materialTextures.push_back(pTexture);
		}
	}
	return hasSuchMap;
}

std::shared_ptr<MaterialTexture> Material::PushTexture
(
	Graphics& Gfx, 
	const std::string& textureFilePath, 
	aiTextureType textureType, 
	UINT bindSlot, 
	wicFlg wicLoadFlags
) 
noexcept
{
	std::string textureFileName = std::filesystem::path(textureFilePath).filename().string();
	int indx = IsLoaded(textureFileName, textureType);
	if (indx >= 0)
		return loadedTextures[indx];
	else
	{
		std::shared_ptr<MaterialTexture> pTexure = std::make_shared<MaterialTexture>(Gfx, textureType, textureFilePath, bindSlot, wicLoadFlags);
		loadedTextures.push_back(pTexure);
		return pTexure;
	}
}

void Material::LoadMaterialProperties(aiMaterial* pMaterial)
{
	for (size_t i = 0; i < pMaterial->mNumProperties; i++)
	{
		auto pProperty = pMaterial->mProperties[i];
		matProps.EmplaceProperty(pProperty->mKey.C_Str(), pProperty->mIndex, pProperty->mType, pProperty->mData, pProperty->mDataLength);
	}
}
