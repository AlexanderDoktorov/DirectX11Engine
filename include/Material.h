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
	alignas(4) bool hasSpecularMapColored;
	alignas(4) bool hasHeightMap;
	dx::XMFLOAT3 Kd; // reflected color diffuse
	dx::XMFLOAT3 Ks; // reflected color specular
	dx::XMFLOAT3 Ka; // reflected color ambient
	dx::XMFLOAT3 Ke; //		      color emissive 
	float  Ns; // shininess
	int illum;
};

struct MapLayout
{
	bool hasNormalMap   : 1 = 0;  // 1 bit for NormalMap
	bool hasDiffuseMap  : 1 = 0;  // 1 bit for DiffuseMap
	bool hasSpecularMap : 1 = 0;  // 1 bit for SpecularMap
	bool hasHeightMap	: 1 = 0;  // 1 bit for HeightMap
	bool hasSpecularMapColored	: 1 = 0;  // 1 bit for SpecularMapColored
	// may add more maps
};

class Material : public IBindable
{
	typedef DirectX::WIC_LOADER_FLAGS wicFlg;
	typedef std::vector<std::shared_ptr<MaterialTexture>>::iterator mIterator;
public:
	Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory);
	
	const std::vector<std::shared_ptr<MaterialTexture>>& GetTextures() const noexcept;

	// Returns true if any property has been changed
	bool ShowMaterialGUI(bool* p_open = (bool*)0);

	virtual void Bind(Graphics& Gfx) noexcept override;
	
	MapLayout	 GetMapLayout() const noexcept;
	bool		 HasAnyMaps() const noexcept;
	std::string  GetName() const noexcept;
	std::string  GetDirectory() const noexcept;
	MaterialPropertiesDesc  GetPropertiesDesc() const noexcept;
	MaterialDesc  GetMaterialDesc() const noexcept;
	bool operator==(const Material& rhs) const noexcept;
private:
	void ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial);
	mIterator LoadMaterialTextures(
		Graphics& Gfx,
		aiMaterial* pMaterial, 
		aiTextureType textureType, 
		UINT bindSlot, 
		wicFlg wicLoadFlags = wicFlg::WIC_LOADER_DEFAULT
	);
	void LoadMaterialProperties(aiMaterial* pMaterial);
private:
	MapLayout   mapLayout;
	std::string materialName;
	std::string materialDirectory;
	std::vector<std::shared_ptr<MaterialTexture>>		   materialTextures;
	MaterialPropertiesDesc matProps;
private:
	static int IsLoaded(const std::string& texturePath,  aiTextureType textureType) noexcept;
	static std::shared_ptr<MaterialTexture> PushTexture(
		Graphics& Gfx, 
		const std::string& texturePath,  
		aiTextureType textureType, 
		UINT bindSlot,
		wicFlg wicLoadFlags) noexcept;
private:
	static std::vector<std::shared_ptr<MaterialTexture>> loadedTextures;
};
