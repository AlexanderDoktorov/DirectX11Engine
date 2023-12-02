#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Texture2D.h"
#include "MaterialProperty.h"

class Material
{
public:
	Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory);
	
	std::shared_ptr<Texture2D> GetTexture(aiTextureType textureType) noexcept;

	template<EMATERIAL_PROPERTY p, typename ReturnType = PDT<p>::type*>
	ReturnType GetProperty() const noexcept
	{
		for (auto& pMatProperty : materialProperties)
		{
			if (pMatProperty->GetPropertyType() == p)
			{
				assert(pMatProperty->GetData() != nullptr && "Material property data was nullptr");
				return reinterpret_cast<ReturnType>(pMatProperty->GetData());
			}
		}
		return nullptr;
	}

	void* GetProperty(EMATERIAL_PROPERTY p) const noexcept
	{
		for (auto& pMatProperty : materialProperties)
		{
			if (pMatProperty->GetPropertyType() == p)
			{
				assert(pMatProperty->GetData() != nullptr && "Material property data was nullptr");
				return pMatProperty->GetData();
			}
		}
		return nullptr;
	}
private:
	void ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial);
	void LoadMaterialTextures(Graphics& Gfx, aiMaterial* pMaterial, aiTextureType textureType);
	void LoadMaterialProperties(aiMaterial* pMaterial);
private:
	std::string materialName;
	std::string materialDirectory;
	std::vector<std::shared_ptr<Texture2D>> materialTextures;
	std::vector<std::shared_ptr<MaterialProperty>> materialProperties;
	
private:
	static int IsLoaded(const std::string& texturePath,  aiTextureType textureType) noexcept;
	static std::shared_ptr<Texture2D> PushTexture(Graphics& Gfx, const std::string& texturePath,  aiTextureType textureType) noexcept;
private:
	static std::vector<std::shared_ptr<Texture2D>> loadedTextures;
};