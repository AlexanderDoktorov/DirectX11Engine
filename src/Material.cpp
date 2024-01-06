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
	if (LoadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE, SLOT_TEXTURE_DIFFUSE)) {
		mapsFlags |= MAP_FLAG_DIFF;
	}
	if (LoadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS, SLOT_TEXTURE_NORMALMAP, WIC_FLAGS_IGNORE_SRGB)) {
		mapsFlags |= MAP_FLAG_NORMAL;
	}
	if (LoadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT, SLOT_TEXTURE_HEIGHT, WIC_FLAGS_IGNORE_SRGB)) {
		mapsFlags |= MAP_FLAG_HEIGHT;
	}
	// Fill specular info
	if (LoadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR,	SLOT_TEXTURE_SPECULAR,  WIC_FLAGS_IGNORE_SRGB))
	{
		mapsFlags |= MAP_FLAG_SPEC;
		auto& p_SpecularTexture = m_Textures.back();
		if (p_SpecularTexture->GetFormat() == DXGI_FORMAT_B8G8R8A8_UNORM || p_SpecularTexture->GetFormat() == DXGI_FORMAT_R8G8B8A8_UNORM) {
			mapsFlags |= MAP_FLAG_SPEC_COLOR;
		}
		if (p_SpecularTexture->AlphaLoaded()) {
			mapsFlags |= MAP_FLAG_SPEC_ALPHA;
		}
	}

	// Fill material desc
	matDesc.FillMapsInfo(mapsFlags);
	LoadMaterialProperties(pMaterial);
}

bool Material::ShowMaterialGUI(bool* p_open)
{
	bool changed = false;
	static constexpr ImVec4 red = {1.f,0.f,0.f,1.f};
	static constexpr ImVec4 yellow = {1.f,1.f,0.f,1.f}; 

	ImGui::TextColored(mapsFlags & MAP_FLAG_DIFF ? yellow : red, "Diffuse map");
	ImGui::TextColored(mapsFlags & MAP_FLAG_HEIGHT ? yellow : red, "Height map");
	ImGui::TextColored(mapsFlags & MAP_FLAG_NORMAL ? yellow : red, "Normal map");
	ImGui::TextColored(mapsFlags & MAP_FLAG_SPEC ? yellow : red, "Specular map");
	ImGui::TextColored(mapsFlags & MAP_FLAG_SPEC_COLOR ? yellow : red, "Specular color map");
	ImGui::TextColored(mapsFlags & MAP_FLAG_SPEC_ALPHA ? yellow : red, "Alpha used");
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

MAP_FLAG Material::GetMapsFlags() const noexcept
{
	return mapsFlags;
}

bool Material::HasAnyMaps() const noexcept
{
	return mapsFlags != 0;
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
