#pragma once
#include <vector>
#include <string>
#include <memory>
#include <DirectXMath.h>
#include "IBindable.h"
#include "Numerated.h"
#include "MaterialTexture.h"
#include "MaterialPropertiesDesc.h"

namespace dx = DirectX;

struct MaterialDesc
{
	alignas(4) bool hasNormalMap;
	alignas(4) bool hasDiffuseMap;
	alignas(4) bool hasSpecularMap;
	alignas(4) bool hasHeightMap;
	dx::XMFLOAT3 Kd; // reflected color diffuse
	dx::XMFLOAT3 Ks; // reflected color specular
	dx::XMFLOAT3 Ka; // reflected color ambient
	float  Ns; // shininess
};

class Material : public IBindable
{
public:
	Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory);
	
	const std::vector<std::shared_ptr<MaterialTexture>>& GetTextures() const noexcept;
	void ShowMaterialControls(const char* label, bool* p_open = (bool*)0);

	virtual void Bind(Graphics& Gfx) noexcept override;
	
	bool HasMap(aiTextureType mapType) const noexcept;
	std::string  GetName() const noexcept;
	std::string  GetDirectory() const noexcept;
	MaterialPropertiesDesc  GetPropertiesDesc() const noexcept;
	MaterialDesc  GetMaterialDesc() const noexcept;
	bool operator==(const Material& rhs) const noexcept;
private:
	void ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial);
	void LoadMaterialTextures(Graphics& Gfx, aiMaterial* pMaterial, aiTextureType textureType, UINT bindSlot);
	void LoadMaterialProperties(aiMaterial* pMaterial);
private: 
	std::string materialName;
	std::string materialDirectory;
	std::vector<std::shared_ptr<MaterialTexture>>		   materialTextures;
	MaterialPropertiesDesc matDesc;
private:
	static int IsLoaded(const std::string& texturePath,  aiTextureType textureType) noexcept;
	static std::shared_ptr<MaterialTexture> PushTexture(Graphics& Gfx, const std::string& texturePath,  aiTextureType textureType, UINT bindSlot) noexcept;
private:
	static std::vector<std::shared_ptr<MaterialTexture>> loadedTextures;
};
