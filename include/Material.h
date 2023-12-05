#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <stdexcept>
#include "Texture2D.h"
#include "MaterialProperty.h"

// Define a helper template with a member type 'type'
template <typename T, typename A = void>
struct HasValueMember : std::false_type {};

// Specialization for the case where 'T::value' is well-formed
template <typename T>
struct HasValueMember<T, std::void_t<decltype(T::value)>> : std::true_type {};

struct MaterialDesc
{
	bool hasNormalMap = false;
	bool pad1[3];
	bool hasDiffuseMap = false;
	bool pad2[3];
	bool hasSpecularMap = false;
	bool pad3[3];
	bool hasHeightMap = false;
	bool pad4[3];
	dx::XMFLOAT3 Kd{}; // color diffuse
	dx::XMFLOAT3 Ks{}; // color specular
	dx::XMFLOAT3 Ka{}; // color ambient
	float Ns{}; // shininess
};

class Material
{
public:
	Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory);
	
	const std::vector<std::shared_ptr<Texture2D>>& GetTextures() const noexcept;
	void ShowControlWindow(Graphics& Gfx);

	template<class T>
	T GetPropertyAs(std::string pKey, [[maybe_unused]] unsigned int = 0, [[maybe_unused]] unsigned int = 0) const noexcept
	{
		static_assert(HasValueMember<PMap<T>>::value, "Trying to get property type with unknown type");
		if constexpr (HasValueMember<PMap<T>>::value)
		{
			for (auto& matProp : materialProperties)
			{
				if (matProp->GetKey() == pKey && PMap<T>::value == matProp->GetType() && matProp->GetSize() == sizeof(T))
				{
					return *reinterpret_cast<const T*>(matProp->GetData().data());
				}
			}
			assert(false && "No matching property found!");
		}
		return T();
	}
	decltype(auto) GetPropertyAsColor(std::string pKey, [[maybe_unused]] unsigned int = 0, [[maybe_unused]] unsigned int = 0) const noexcept
	{
		return GetPropertyAs<aiColor3D>(pKey);
	}
	dx::XMFLOAT3 GetPropertyAsColorDx(std::string pKey, [[maybe_unused]] unsigned int, [[maybe_unused]] unsigned int) const noexcept
	{
		auto data = GetPropertyAsColor(pKey);
		return *reinterpret_cast<dx::XMFLOAT3*>(&data);
	}
	MaterialDesc GetDesc() const noexcept;
	int  GetIndex() const noexcept;
	bool HasNormalMaps() const noexcept;
	bool HasDiffuseMaps() const noexcept;
	bool HasSpecularMaps() const noexcept;
	bool HasHeightMaps() const noexcept;
private:
	void ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial);
	void LoadMaterialTextures(Graphics& Gfx, aiMaterial* pMaterial, aiTextureType textureType);
	void LoadMaterialProperties(aiMaterial* pMaterial);
private:
	std::string materialName;
	std::string materialDirectory;
	std::vector<std::shared_ptr<Texture2D>>		   materialTextures;
	std::vector<std::shared_ptr<MaterialProperty>> materialProperties;

	MaterialDesc matDesc    = {};
	int  materialIndex      = 0;
private:
	static int IsLoaded(const std::string& texturePath,  aiTextureType textureType) noexcept;
	static std::shared_ptr<Texture2D> PushTexture(Graphics& Gfx, const std::string& texturePath,  aiTextureType textureType) noexcept;
private:
	static std::vector<std::shared_ptr<Texture2D>> loadedTextures;
	static int materialsCount;
};

