#pragma once
#include <vector>
#include <string>
#include <memory>
#include <DirectXMath.h>
#include "WICTexture.h"
#include "MaterialPropertiesDesc.h"
#include <PixelConstantBuffer.h>

namespace dx = DirectX;

struct MapLayout
{
	bool hasNormalMap			: 1 = 0;  // 1 bit for NormalMap
	bool hasDiffuseMap			: 1 = 0;  // 1 bit for DiffuseMap
	bool hasHeightMap			: 1 = 0;  // 1 bit for HeightMap
	bool hasSpecularPowerMap	: 1 = 0;  // 1 bit for SpecularPowerMap
	bool hasSpecularColorMap	: 1 = 0;  // 1 bit for SpecularColorMap channel
	bool hasSpecularAlpha		: 1 = 0;  // 1 bit for SpecularAlpha channel
	// may add more maps
};

struct MaterialDesc
{
	BOOL useNormalMap		= FALSE;
	BOOL useDiffuseMap		= FALSE;
	BOOL useHeightMap		= FALSE;
	BOOL useSpecOnlyRed		= FALSE;
	BOOL useSpecColored		= FALSE;
	BOOL hasSpecularAlpha	= FALSE;
	int32_t		 illum{3};
	float		 Ns{}; // shininess
	alignas(16) dx::XMFLOAT3 Kd{}; // reflected color diffuse
	alignas(16) dx::XMFLOAT3 Ks{}; // reflected color specular
	alignas(16) dx::XMFLOAT3 Ka{}; // reflected color ambient
	alignas(16) dx::XMFLOAT3 Ke{}; //	color emissive 

	MaterialDesc() = default;
	void FillMapsInfo(const MapLayout& mapLayout) noexcept
	{
		useNormalMap	 = mapLayout.hasNormalMap;
		useDiffuseMap	 = mapLayout.hasDiffuseMap;
		useHeightMap	 = mapLayout.hasHeightMap;
		useSpecOnlyRed	 = mapLayout.hasSpecularPowerMap;
		useSpecColored	 = mapLayout.hasSpecularColorMap;
		hasSpecularAlpha = mapLayout.hasSpecularAlpha;
	}
};

class Material : public IBindable
{
	friend class Mesh;
public:
	using wicFlg = DirectX::WIC_FLAGS;
	using strbuff_type = MaterialDesc;

	Material(Graphics& Gfx, const aiMaterial* pMaterial, std::string materialDirectory);
	
	// Returns true if any property has been changed
	bool ShowMaterialGUI(bool* p_open = (bool*)0);

	virtual void Bind(Graphics& Gfx) noexcept override;
	
	MapLayout	 GetMapLayout() const noexcept;
	bool		 HasAnyMaps() const noexcept;
	std::string  GetName() const noexcept;
	std::string  GetDirectory() const noexcept;
	std::string  GetPath() const noexcept;
	MaterialDesc GetMaterialDesc() const noexcept;
	bool operator==(const Material& rhs) const noexcept;
private:
	void ProcessMaterial(Graphics& Gfx, const aiMaterial* pMaterial);
	bool LoadMaterialTextures(
		Graphics& Gfx,
		const aiMaterial* pMaterial, 
		aiTextureType textureType, 
		UINT bindSlot, 
		wicFlg wicLoadFlags = wicFlg::WIC_FLAGS_NONE
	);
	void LoadMaterialProperties(const aiMaterial* pMaterial);
private:
	std::string materialName;
	std::string materialDirectory;
	MaterialDesc matDesc;
	std::vector<std::shared_ptr<WICTexture>> m_Textures;
	static std::shared_ptr<PixelConstantBuffer<MaterialDesc>> sp_MaterialBuffer;
	MapLayout mapLayout;
};
