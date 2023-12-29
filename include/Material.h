#pragma once
#include <vector>
#include <string>
#include <memory>
#include <DirectXMath.h>
#include "IBindable.h"
#include "Numerated.h"
#include "WICTexture.h"
#include "MaterialPropertiesDesc.h"

namespace dx = DirectX;

struct MaterialDesc
{
	alignas(4) bool hasNormalMap;
	alignas(4) bool hasDiffuseMap;
	alignas(4) bool hasSpecularMap;
	alignas(4) bool hasSpecularAlpha;
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
	bool hasSpecularAlpha	: 1 = 0;  // 1 bit for SpecularAlpha channel
	// may add more maps
};

static std::mutex mtx;

class Material : public IBindable
{
public:
	using wicFlg = DirectX::WIC_FLAGS;
	using strbuff_type = MaterialDesc;

	Material(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory);
	
	// Returns true if any property has been changed
	bool ShowMaterialGUI(bool* p_open = (bool*)0);

	virtual void Bind(Graphics& Gfx) noexcept override;
	static std::string GenerateID(Graphics& Gfx, aiMaterial* pMaterial, std::string materialDirectory) noexcept;
	
	MapLayout	 GetMapLayout() const noexcept;
	bool		 HasAnyMaps() const noexcept;
	std::string  GetName() const noexcept;
	std::string  GetDirectory() const noexcept;
	std::string  GetPath() const noexcept;
	MaterialPropertiesDesc  GetPropertiesDesc() const noexcept;
	MaterialDesc  GetMaterialDesc() const noexcept;
	bool operator==(const Material& rhs) const noexcept;
private:
	void ProcessMaterial(Graphics& Gfx, aiMaterial* pMaterial);
	bool LoadMaterialTextures(
		Graphics& Gfx,
		aiMaterial* pMaterial, 
		aiTextureType textureType, 
		UINT bindSlot, 
		wicFlg wicLoadFlags = wicFlg::WIC_FLAGS_NONE
	);
	void LoadMaterialProperties(aiMaterial* pMaterial);
private:
	MapLayout   mapLayout;
	std::string materialName;
	std::string materialDirectory;
	std::vector<std::shared_ptr<WICTexture>> m_Textures;
	std::vector<std::future<void>> m_Futures;
	MaterialPropertiesDesc matProps;
private:
};
