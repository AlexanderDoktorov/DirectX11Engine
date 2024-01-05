#pragma once
#include <vector>
#include <string>
#include <memory>
#include <DirectXMath.h>
#include "WICTexture.h"
#include "MaterialPropertiesDesc.h"
#include <PixelConstantBuffer.h>

namespace dx = DirectX;

enum MAP_FLAG : uint32_t
{
	MAP_FLAG_DIFF		= 1 << 0,
	MAP_FLAG_NORMAL		= 1 << 1,
	MAP_FLAG_SPEC_POWER	= 1 << 2,
	MAP_FLAG_SPEC_COLOR	= 1 << 3,
	MAP_FLAG_SPEC_ALPHA	= 1 << 4,
	MAP_FLAG_HEIGHT		= 1 << 5,
	MAP_FLAG_SPEC		= MAP_FLAG_SPEC_POWER | MAP_FLAG_SPEC_COLOR,
};

static inline MAP_FLAG& operator|=(MAP_FLAG& lhs, MAP_FLAG rhs) {
	using T = std::underlying_type_t<MAP_FLAG>;
	lhs = static_cast<MAP_FLAG>(static_cast<T>(lhs) | static_cast<T>(rhs));
	return lhs;
}

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
	void FillMapsInfo(const MAP_FLAG& mapsFlags) noexcept
	{
		useNormalMap	 = mapsFlags & MAP_FLAG_NORMAL;
		useDiffuseMap	 = mapsFlags & MAP_FLAG_DIFF;
		useHeightMap	 = mapsFlags & MAP_FLAG_HEIGHT;
		useSpecOnlyRed	 = mapsFlags & MAP_FLAG_SPEC_POWER;
		useSpecColored	 = mapsFlags & MAP_FLAG_SPEC_COLOR;
		hasSpecularAlpha = mapsFlags & MAP_FLAG_SPEC_ALPHA;
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
	
	MAP_FLAG	 GetMapsFlags() const noexcept;
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
	MAP_FLAG mapsFlags{};
};
