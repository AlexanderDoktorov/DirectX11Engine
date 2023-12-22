#pragma once
#include "Graphics.h"
#include "Material.h"
#include <algorithm>



class MaterialSystem : public GraphicsChild
{
public:
	using m_type = Material;
	using buff_data_type  = MaterialDesc;

	MaterialSystem(Graphics& Gfx, const RECT& windowRect);

	const ID3D11RenderTargetView* GetMaterialRTV(Graphics& Gfx) const noexcept;
	const ID3D11ShaderResourceView* GetMaterialTexure(Graphics& Gfx) const noexcept;
	const ID3D11ShaderResourceView* GetMaterialBuffer(Graphics& Gfx) const noexcept;

	size_t ResolveMaterial(Graphics& Gfx, aiMaterial* pMaterial, const std::string& materialDirectory) noexcept
	{
		return ResolveMaterialImpl(Gfx, pMaterial, materialDirectory);
	}

	template<class... Args>
	size_t ResolveMaterialImpl(Graphics& Gfx, Args&&... args) noexcept
	{
		std::string mStrId = m_type::GenerateID(Gfx, args...);

		if (auto it = mapIndex.find(mStrId); it != mapIndex.end())
			return it->second;
		else
		{
			assert(vMaterials.size() < maxSize);
			const size_t newIndx = vMaterials.size();
			mapIndex.emplace(std::make_pair(std::move(mStrId), newIndx));
			vMaterials.push_back(std::make_shared<m_type>(Gfx, std::forward<Args>(args)...));
			UpdateBuffAt(Gfx, vMaterials.back()->GetMaterialDesc(), newIndx);
			return newIndx;
		}
	}

	HRESULT InitilizeMaterialBuffer(Graphics& Gfx, UINT maxMaterials, const buff_data_type& initialValue = buff_data_type() ) noexcept;
private:
	void UpdateBuffAt(Graphics& Gfx, const buff_data_type& data, size_t indx);

	UINT maxSize = 0U;
	std::unordered_map<std::string, size_t> mapIndex;
	std::vector<std::shared_ptr<m_type>> vMaterials;
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRtv_mId;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSrv_mId;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pText2D_mID;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pStrbuff;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSrv_strbuff;
};
