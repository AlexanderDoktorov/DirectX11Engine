#pragma once
#include "Material.h"
#include <algorithm>

class MaterialSystem : public GraphicsChild
{
	friend class Graphics;
public:
	using m_type = Material;
	using buff_data_type  = MaterialDesc;
	using buff_format_type = int16_t; // -1 as a sign of empty material
	static constexpr DXGI_FORMAT buff_format = DXGI_FORMAT_R16_SINT; // -1 as a sign of empty material
public:
	MaterialSystem(Graphics& Gfx, const RECT& windowRect);

	HRESULT InitilizeMaterialBuffer(Graphics& Gfx, UINT maxMaterials, const buff_data_type& initialValue = buff_data_type() ) noexcept;
	HRESULT OnResize(Graphics& Gfx, const RECT& windowRect) noexcept;

	void    ShowMaterialsWindow(Graphics& Gfx, bool* p_open = (bool*)1) noexcept;
	void	UpdateMaterialAt(Graphics& Gfx, const size_t& index)		noexcept;

	const ID3D11RenderTargetView*	GetMaterialRTV(Graphics& Gfx)		const noexcept;
	const ID3D11ShaderResourceView* GetMaterialTexure(Graphics& Gfx)	const noexcept;
	const ID3D11ShaderResourceView* GetMaterialBuffer(Graphics& Gfx)	const noexcept;
	std::shared_ptr<m_type> GetMaterialByIndex(const size_t& index)		const noexcept;

	size_t ResolveMaterial(Graphics& Gfx, aiMaterial* pMaterial, const std::string& materialDirectory) noexcept;
	MaterialSystem& ClearRenderTarget(Graphics& Gfx, const buff_format_type& clearValue = -1) noexcept;

private:
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
