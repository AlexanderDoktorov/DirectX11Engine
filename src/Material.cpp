#include "Material.h"
#include "SlotLayout.h"
#include <filesystem>

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
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE,		SLOT_TEXTURE_DIFFUSE);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS,		SLOT_TEXTURE_NORMALMAP);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR,	SLOT_TEXTURE_SPECULAR);
	LoadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT,		SLOT_TEXTURE_HEIGHT);
	LoadMaterialProperties(pMaterial);
}

const std::vector<std::shared_ptr<MaterialTexture>>& Material::GetTextures() const noexcept
{
	return materialTextures;
}

void Material::ShowMaterialControls(const char* label, bool* p_open)
{
	if (ImGui::Begin(label, p_open))
	{
		matDesc.ShowGUI();
	}
	ImGui::End();
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

bool Material::HasMap(aiTextureType mapType) const noexcept
{
	for (const auto& materialTex : materialTextures)
		if (materialTex->GetTextureAiType() == mapType)
			return true;
	return false;
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
	return matDesc;
}
MaterialDesc Material::GetMaterialDesc() const noexcept
{
	MaterialDesc materialDesc_{};
	materialDesc_.hasDiffuseMap = HasMap(aiTextureType_DIFFUSE);
	materialDesc_.hasHeightMap = HasMap(aiTextureType_HEIGHT);
	materialDesc_.hasNormalMap = HasMap(aiTextureType_NORMALS);
	materialDesc_.hasSpecularMap = HasMap(aiTextureType_SPECULAR);
	materialDesc_.Ka = reinterpret_cast<dx::XMFLOAT3&&>(matDesc.GetPropertyAs<aiColor3D>(AI_MATKEY_COLOR_AMBIENT).value());
	materialDesc_.Kd = reinterpret_cast<dx::XMFLOAT3&&>(matDesc.GetPropertyAs<aiColor3D>(AI_MATKEY_COLOR_DIFFUSE).value());
	materialDesc_.Ks = reinterpret_cast<dx::XMFLOAT3&&>(matDesc.GetPropertyAs<aiColor3D>(AI_MATKEY_COLOR_SPECULAR).value());
	return materialDesc_;
}
bool Material::operator==(const Material& rhs) const noexcept
{
	return materialName == rhs.materialName && materialDirectory == rhs.materialDirectory;
}

std::shared_ptr<MaterialTexture> Material::PushTexture(Graphics& Gfx, const std::string& textureFilePath, aiTextureType textureType, UINT bindSlot) noexcept
{
	std::string textureFileName = std::filesystem::path(textureFilePath).filename().string();
	int indx = IsLoaded(textureFileName, textureType);
	if (indx >= 0)
		return loadedTextures[indx];
	else
	{
		std::shared_ptr<MaterialTexture> pTexure = std::make_shared<MaterialTexture>(Gfx, textureType, textureFilePath, bindSlot);
		loadedTextures.push_back(pTexure);
		return pTexure;
	}
}

void Material::LoadMaterialTextures(Graphics& Gfx, aiMaterial* pMaterial, aiTextureType textureType, UINT bindSlot)
{
	for (unsigned int i = 0u; i < pMaterial->GetTextureCount(textureType); i++)
	{
		aiString textureFileName;
		if (pMaterial->GetTexture(textureType, i, &textureFileName) == aiReturn_SUCCESS)
		{
			const std::string textureFilePath =  materialDirectory + '\\' + std::string(textureFileName.C_Str());
			auto pTexture = PushTexture(Gfx, textureFilePath, textureType, bindSlot);
			materialTextures.push_back(pTexture);
		}
	}
}

void Material::LoadMaterialProperties(aiMaterial* pMaterial)
{
	for (size_t i = 0; i < pMaterial->mNumProperties; i++)
	{
		auto pProperty = pMaterial->mProperties[i];
		matDesc.EmplaceProperty(pProperty->mKey.C_Str(), pProperty->mIndex, pProperty->mType, pProperty->mData, pProperty->mDataLength);
	}
}
