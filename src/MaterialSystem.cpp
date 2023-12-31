#include "MaterialSystem.h"

MaterialSystem::MaterialSystem(Graphics& Gfx, const RECT& windowRect)
{
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Height =  static_cast<UINT>(std::abs(windowRect.bottom - windowRect.top));
	textureDesc.Width =  static_cast<UINT>(std::abs(windowRect.right - windowRect.left));
	textureDesc.Format = buff_format; 
	textureDesc.MipLevels = 1U; // only mos detailed mip level
	textureDesc.ArraySize = 1U;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0U;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.MiscFlags = 0U;
	textureDesc.SampleDesc.Count = 1U;
	textureDesc.SampleDesc.Quality = 0U;

	int16_t* arrPixels = new int16_t[textureDesc.Width * textureDesc.Height];
	std::fill_n(arrPixels, textureDesc.Width * textureDesc.Height, -1); // fill texture with -1 values

	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = arrPixels;
	initialData.SysMemPitch = sizeof(int16_t) * textureDesc.Width;
	initialData.SysMemSlicePitch = 0U;

	HRESULT hr = GetDevice(Gfx)->CreateTexture2D(&textureDesc, &initialData, &pText2D_mID);
	assert(SUCCEEDED(hr));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0U;

	hr = GetDevice(Gfx)->CreateRenderTargetView(pText2D_mID.Get(), &rtvDesc, &pRtv_mId);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0U;
	hr = GetDevice(Gfx)->CreateShaderResourceView(pText2D_mID.Get(), &srvDesc, &pSrv_mId);
	assert(SUCCEEDED(hr));

	delete[] arrPixels;
}

const ID3D11RenderTargetView* MaterialSystem::GetMaterialRTV(Graphics& Gfx) const noexcept
{
	return pRtv_mId.Get(); 
}

const ID3D11ShaderResourceView* MaterialSystem::GetMaterialTexure(Graphics& Gfx) const noexcept
{
	return pSrv_mId.Get();
}

const ID3D11ShaderResourceView* MaterialSystem::GetMaterialBuffer(Graphics& Gfx) const noexcept
{ 
	return pSrv_strbuff.Get(); 
}

std::shared_ptr<MaterialSystem::m_type> MaterialSystem::GetMaterialByIndex(const size_t& index) const noexcept
{
	return vMaterials.at(index);
}

void MaterialSystem::UpdateMaterialAt(Graphics& Gfx, const size_t& index) noexcept
{
	UpdateBuffAt(Gfx, GetMaterialByIndex(index)->GetMaterialDesc(), index);
}

size_t MaterialSystem::ResolveMaterial(Graphics& Gfx, aiMaterial* pMaterial, const std::string& materialDirectory) noexcept
{
	return ResolveMaterialImpl(Gfx, pMaterial, materialDirectory);
}

MaterialSystem& MaterialSystem::ClearRenderTarget(Graphics& Gfx, const buff_format_type& clearValue) noexcept
{
	const FLOAT asFloat = static_cast<FLOAT>(clearValue);
	const FLOAT clearColor[4] = { asFloat, asFloat, asFloat, asFloat };
	GetContext(Gfx)->ClearRenderTargetView(pRtv_mId.Get(), clearColor);
	return *this;
}

HRESULT MaterialSystem::OnResize(Graphics& Gfx, const RECT& windowRect) noexcept
{
	D3D11_TEXTURE2D_DESC textureDesc;
	pText2D_mID->GetDesc(&textureDesc);

	// Resize texture
	textureDesc.Height =  static_cast<UINT>(std::abs(windowRect.bottom - windowRect.top));
	textureDesc.Width =  static_cast<UINT>(std::abs(windowRect.right - windowRect.left));

	HRESULT hr = GetDevice(Gfx)->CreateTexture2D(&textureDesc, nullptr, &pText2D_mID);
	if (FAILED(hr))
		return hr;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; 
	rtvDesc.Texture2D.MipSlice = 0U; 

	hr = GetDevice(Gfx)->CreateRenderTargetView(pText2D_mID.Get(), &rtvDesc, &pRtv_mId); 
	if (FAILED(hr))
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{}; 
	srvDesc.Format = textureDesc.Format; 
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; 
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels; 
	srvDesc.Texture2D.MostDetailedMip = 0U; 
	hr = GetDevice(Gfx)->CreateShaderResourceView(pText2D_mID.Get(), &srvDesc, &pSrv_mId); 

	return hr;
}

void MaterialSystem::ShowMaterialsWindow(Graphics& Gfx, bool* p_open) noexcept
{
	if (ImGui::Begin("Loaded materials", p_open))
	{
		for (size_t i = 0; i < vMaterials.size(); i++)
		{
			if (vMaterials[i]->ShowMaterialGUI())
				UpdateMaterialAt(Gfx, i);
			ImGui::Separator();
		}
	}
	ImGui::End();
}

HRESULT MaterialSystem::InitilizeMaterialBuffer(Graphics& Gfx, UINT maxMaterials, const buff_data_type& initialValue) noexcept
{
	maxSize = maxMaterials;

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth = sizeof(buff_data_type) * maxMaterials;
	bufferDesc.StructureByteStride = sizeof(buff_data_type);
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	std::vector<buff_data_type> emptymaterials(maxMaterials);
	std::fill(emptymaterials.begin(), emptymaterials.end(), initialValue);
	D3D11_SUBRESOURCE_DATA CBSubData = {};
	CBSubData.pSysMem = emptymaterials.data();

	HRESULT hr = GetDevice(Gfx)->CreateBuffer(&bufferDesc, &CBSubData, &pStrbuff);
	if (FAILED(hr))
		return hr;

	// Create the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffers
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.NumElements = maxMaterials;

	hr = GetDevice(Gfx)->CreateShaderResourceView(pStrbuff.Get(), &srvDesc, &pSrv_strbuff);
	return hr;
}

void MaterialSystem::UpdateBuffAt(Graphics& Gfx, const buff_data_type& data, size_t indx)
{
	assert(indx < maxSize);
	D3D11_MAPPED_SUBRESOURCE mappedSubresource{};
	HRESULT hr = GetContext(Gfx)->Map(pStrbuff.Get(), 0U, D3D11_MAP_WRITE_NO_OVERWRITE , 0U, &mappedSubresource); assert(SUCCEEDED(hr));

	// Calculate the offset to the particular struct
	size_t offset = indx * sizeof(MaterialDesc);

	// Copy the new data to the mapped resource
	::memcpy(static_cast<char*>(mappedSubresource.pData) + offset, &data, sizeof(MaterialDesc));

	GetContext(Gfx)->Unmap(pStrbuff.Get(), 0U);
}