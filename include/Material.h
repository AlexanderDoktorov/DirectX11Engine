#pragma once
#include <vector>
#include <string>
#include <type_traits>
#include "IBindable.h"
#include "Numerated.h"
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
	float Ns{};		   // shininess
};

class Material
{
public:
	Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory, size_t materialIndex);
	
	const std::vector<std::shared_ptr<Texture2D>>& GetTextures() const noexcept;
	void ShowMaterialControls(Graphics& Gfx);

	template<class T>
	std::optional<T> GetPropertyAs(std::string pKey, [[maybe_unused]] unsigned int = 0, [[maybe_unused]] unsigned int = 0) const noexcept(!_DEBUG)
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
		}
		assert(false && "Property was not found");
		return std::nullopt;
	}
	size_t GetIndex() const noexcept;
	MaterialDesc GetDesc() const noexcept;
	std::string  GetName() const noexcept;
	std::string  GetDirectory() const noexcept;
	bool HasNormalMaps() const noexcept;
	bool HasDiffuseMaps() const noexcept;
	bool HasSpecularMaps() const noexcept;
	bool HasHeightMaps() const noexcept;
	bool operator==(const Material& rhs) const noexcept;
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
	size_t materialIndex    = {};
private:
	static int IsLoaded(const std::string& texturePath,  aiTextureType textureType) noexcept;
	static std::shared_ptr<Texture2D> PushTexture(Graphics& Gfx, const std::string& texturePath,  aiTextureType textureType) noexcept;
private:
	static std::vector<std::shared_ptr<Texture2D>> loadedTextures;
};

