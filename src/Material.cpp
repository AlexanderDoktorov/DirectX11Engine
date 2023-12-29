#include "Material.h"
#include "SlotLayout.h"
#include <filesystem>
#include <iostream>
#include <ranges>
#include <future>
#include "DOK_assimp.h"

Material::Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory) 
	: 
	materialName(pMaterial->GetName().C_Str()), 
	materialDirectory(materialDirectory)
{
	ProcessMaterial(Gfx, pMaterial);
}

void Material::ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial)
{
	using enum WICTexture::wicFlg;
	mapLayout.hasDiffuseMap  = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE,		SLOT_TEXTURE_DIFFUSE);
	mapLayout.hasNormalMap   = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS,		SLOT_TEXTURE_NORMALMAP, WIC_FLAGS_IGNORE_SRGB);
	mapLayout.hasSpecularMap = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR,		SLOT_TEXTURE_SPECULAR, WIC_FLAGS_IGNORE_SRGB);
	mapLayout.hasHeightMap   = LoadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT,		SLOT_TEXTURE_HEIGHT);
	LoadMaterialProperties(pMaterial);
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
	ImGui::TextColored(mapLayout.hasSpecularAlpha ? yellow : red, "Alpha used");
	return changed;
}

void Material::Bind(Graphics& Gfx) noexcept
{
	std::ranges::for_each(m_Textures, [&Gfx](auto& val) {
		val->Bind(Gfx);
	});
}

std::string Material::GenerateID(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory) noexcept
{
	return std::string(typeid(Material).name()) + pMaterial->GetName().C_Str() + materialDirectory;
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
std::string Material::GetPath() const noexcept
{
	return materialDirectory + "\\" + materialName;
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
	materialDesc_.hasSpecularAlpha = mapLayout.hasSpecularAlpha;
	materialDesc_.Ka = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_AMBIENT).value();
	materialDesc_.Kd = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_DIFFUSE).value();
	materialDesc_.Ks = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_SPECULAR).value();
	materialDesc_.Ke = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_EMISSIVE).value();
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
#define ASYNC 1
	for (unsigned int i = 0u; i < pMaterial->GetTextureCount(textureType); i++)
	{
		aiString textureFileName;
		if (pMaterial->GetTexture(textureType, i, &textureFileName) == aiReturn_SUCCESS)
		{
			const std::string textureFilePath =  materialDirectory + '\\' + std::string(textureFileName.C_Str());
			m_Textures.push_back(WICTexture::Resolve(Gfx, textureFilePath.c_str(), bindSlot, wicFlags));
		}
	}
	return pMaterial->GetTextureCount(textureType) != 0;
}

void Material::LoadMaterialProperties(aiMaterial* pMaterial)
{
	for (size_t i = 0; i < pMaterial->mNumProperties; i++)
	{
		auto pProperty = pMaterial->mProperties[i];
		matProps.EmplaceProperty(pProperty->mKey.C_Str(), pProperty->mIndex, pProperty->mType, pProperty->mData, pProperty->mDataLength);
	}
}
