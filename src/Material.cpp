#include "Material.h"
#include "SlotLayout.h"
#include <filesystem>
#include <iostream>
#include <ranges>
#include "DOK_assimp.h"
#include <BindableSystem.h>

std::shared_ptr<PixelConstantBuffer<MaterialDesc>> Material::sp_MaterialBuffer{};

Material::Material(Graphics& Gfx, const aiMaterial* pMaterial, std::string materialDirectory) 
	: 
	materialName(pMaterial->GetName().C_Str()), 
	materialDirectory(materialDirectory)
{
	if (!sp_MaterialBuffer)
		sp_MaterialBuffer = PixelConstantBuffer<MaterialDesc>::Resolve(Gfx, SLOT_BUFFER_MATERIAL);
	ProcessMaterial(Gfx, pMaterial);
}

void Material::ProcessMaterial(Graphics& Gfx, const aiMaterial* pMaterial)
{
	using enum WICTexture::wicFlg;
	mapLayout.hasDiffuseMap =  LoadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE,	SLOT_TEXTURE_DIFFUSE);
	mapLayout.hasNormalMap	=  LoadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS,	SLOT_TEXTURE_NORMALMAP, WIC_FLAGS_IGNORE_SRGB);
	mapLayout.hasHeightMap	=  LoadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT,	SLOT_TEXTURE_HEIGHT,    WIC_FLAGS_IGNORE_SRGB);

	// Fill specular info
	if (LoadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR,	SLOT_TEXTURE_SPECULAR,  WIC_FLAGS_IGNORE_SRGB))
	{
		auto& p_SpecularTexture = m_Textures.back();
		mapLayout.hasSpecularColorMap = p_SpecularTexture->GetFormat() == DXGI_FORMAT_B8G8R8A8_UNORM || p_SpecularTexture->GetFormat() == DXGI_FORMAT_R8G8B8A8_UNORM;
		mapLayout.hasSpecularPowerMap = !mapLayout.hasSpecularColorMap; // if isn't colored -> it is only red channel map (probably)
		mapLayout.hasSpecularAlpha	  = p_SpecularTexture->AlphaLoaded();
	}

	// Fill material desc
	matDesc.FillMapsInfo(mapLayout);
	LoadMaterialProperties(pMaterial);
}

bool Material::ShowMaterialGUI(bool* p_open)
{
	bool changed = false;
	static constexpr ImVec4 red = {1.f,0.f,0.f,1.f};
	static constexpr ImVec4 yellow = {1.f,1.f,0.f,1.f}; 

	auto makeHashed = [](std::string str, const std::string_view& hash) -> std::string {
		return str.append("##").append(hash);
	};

	if(mapLayout.hasDiffuseMap)
		changed |= ImGui::Checkbox(makeHashed("Use diffuse map", materialName).c_str(), reinterpret_cast<bool*>(&matDesc.useDiffuseMap));
	if(!matDesc.useDiffuseMap)
		changed |= ImGui::ColorEdit3(makeHashed("Material albedo color", materialName).c_str(), &matDesc.Kd.x);
	if(mapLayout.hasNormalMap)
		changed |= ImGui::Checkbox(makeHashed("Use normal map", materialName).c_str(), reinterpret_cast<bool*>(&matDesc.useNormalMap));
	if(mapLayout.hasSpecularPowerMap)
		changed |= ImGui::Checkbox(makeHashed("Specular power map", materialName).c_str(), reinterpret_cast<bool*>(&matDesc.useSpecOnlyRed));
	if(mapLayout.hasSpecularColorMap)
		changed |= ImGui::Checkbox(makeHashed("Specular color map", materialName).c_str(), reinterpret_cast<bool*>(&matDesc.useSpecColored));
	
	ImGui::TextColored(mapLayout.hasDiffuseMap ? yellow : red, "Diffuse map");
	ImGui::TextColored(mapLayout.hasHeightMap ? yellow : red, "Height map");
	ImGui::TextColored(mapLayout.hasNormalMap ? yellow : red, "Normal map");
	ImGui::TextColored(mapLayout.hasSpecularPowerMap ? yellow : red, "Specular power map");
	ImGui::TextColored(mapLayout.hasSpecularColorMap ? yellow : red, "Specular color map");
	ImGui::TextColored(mapLayout.hasSpecularAlpha ? yellow : red, "Alpha used");
	return changed;
}

void Material::Bind(Graphics& Gfx) noexcept
{
	std::ranges::for_each(m_Textures, [&Gfx](auto& val) {
		val->Bind(Gfx);
	});
	sp_MaterialBuffer->Update(Gfx, matDesc);
	sp_MaterialBuffer->Bind(Gfx);
}

MapLayout Material::GetMapLayout() const noexcept
{
	return mapLayout;
}

bool Material::HasAnyMaps() const noexcept
{
	return mapLayout.hasDiffuseMap || mapLayout.hasHeightMap || mapLayout.hasNormalMap || mapLayout.hasSpecularColorMap || mapLayout.hasSpecularPowerMap;
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
MaterialDesc Material::GetMaterialDesc() const noexcept
{
	return matDesc;
}
bool Material::operator==(const Material& rhs) const noexcept
{
	return materialName == rhs.materialName && materialDirectory == rhs.materialDirectory;
}

bool Material::LoadMaterialTextures
(
	Graphics& Gfx, 
	const aiMaterial* pMaterial, 
	aiTextureType textureType, 
	UINT bindSlot, 
	wicFlg wicFlags
)
{
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

void Material::LoadMaterialProperties(const aiMaterial* pMaterial)
{
	MaterialPropertiesDesc matProps;
	for (size_t i = 0; i < pMaterial->mNumProperties; i++)
	{
		auto pProperty = pMaterial->mProperties[i];
		matProps.EmplaceProperty(pProperty->mKey.C_Str(), pProperty->mIndex, pProperty->mType, pProperty->mData, pProperty->mDataLength);
	}

	constexpr DirectX::XMFLOAT3 color_default = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	matDesc.Ka = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_AMBIENT).value_or(color_default);
	matDesc.Kd = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_DIFFUSE).value_or(color_default);
	matDesc.Ks = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_SPECULAR).value_or(color_default);
	matDesc.Ke = matProps.GetPropertyAs<DirectX::XMFLOAT3>(AI_MATKEY_COLOR_EMISSIVE).value_or(color_default);
	matDesc.Ns = matProps.GetPropertyAs<float>(AI_MATKEY_SHININESS).value_or(10.f);
	if (auto prop = matProps.GetProperty(AI_MATKEY_SHADING_MODEL)) {
		matDesc.illum = *(int*)prop->GetData().data();
	}
}
