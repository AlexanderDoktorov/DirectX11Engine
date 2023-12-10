#pragma once
#include <DirectXMath.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include <array>
#include <optional>
#include "assimp\material.h"
#include "DirectXWindow.h"
#include "Camera.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"

#include "IBindable.h"
#include "StructuredBuffer.h"
#include "VertexShaderCommon.h"
#include "PixelShaderCommon.h"
#include "PixelConstantBuffer.h"
#include "RenderTexture.h"
#include "Interfaces.h"
#include "Material.h"
#include "Sampler.h"
#include "noxnd.h"

namespace wrl = Microsoft::WRL;
namespace dx  = DirectX;

#define MAX_MATERIALS (size_t)20

class Graphics
{
public:
	// Meta info
	using MaterialBuffer = StructuredBuffer<MaterialDesc, MAX_MATERIALS, PSResourse>;
private:
	friend class GraphicsChild;
	class MaterialSystem
	{
		friend class Graphics;
	public:
		MaterialSystem() = default;
		void Initilize(Graphics& Gfx, const RECT& rc) noexcept;
		void OnResize(UINT resizeWidth, UINT resizeHeight) noexcept;
		size_t GetMaterialIndex(Material& material) noxnd;
		bool UpdateMaterialAt(size_t indx) noexcept;
		Material* GetMaterialAt(size_t indx) noexcept;
		std::optional<size_t> IsLoaded(const Material& material) const noexcept;
		void ShowMaterialsWindow(bool* p_open = (bool*)1) noexcept;

	private:
		wrl::ComPtr<ID3D11RenderTargetView>		rtvMaterialID;
		std::unique_ptr<MaterialBuffer>			pMaterialBuffer;
		std::unique_ptr<RenderTexture>			MaterialIDTexture;
		std::vector<std::unique_ptr<Material>>	loadedMaterials;
		Graphics* pGfx = nullptr;
	};
public:
	Graphics(HWND hwnd);

	void BeginFrame(const DirectXWindow* pWnd, const float clear_color[4]);

	// Deffered Rendering
	void			ResizeRenderTargetViews(const DirectXWindow* pWnd);

	void			BeginGeometryPass(const DirectXWindow* pWnd, const float clear_color[4]);
	void			EndGeometryPass();

	void			BeginLightningPass();
	void			EndLightningPass();

	void			PerformCombinePass();

	void			SetProjection(dx::XMMATRIX projection) noexcept;
	void			SetCamera(const Camera& cam);

	void			SetAdditiveBlendingState();
	void			ResetBlendingState();
	void			EndFrame();
	void			DrawIndexed(UINT Count);
	void			Draw(UINT vertex_count);
	void			RenderToImGui(const bool& state);

	// Materials
	MaterialSystem& GetMaterialSystem() noexcept;

	// Render targets
	wrl::ComPtr<ID3D11ShaderResourceView> MakeSRVFromRTV(wrl::ComPtr<ID3D11RenderTargetView> rtv);
	static wrl::ComPtr<ID3D11RenderTargetView> MakeRTVFromTexture(ID3D11Device* p_Device, const ITexture2D* texture);

	Camera			GetCamera()		const;
	dx::XMMATRIX	GetProjection() const noexcept;

	~Graphics();

private:
	dx::XMMATRIX	projection = dx::XMMatrixIdentity();
	Camera			cam = Camera();

	void			RecreateMainViews(const UINT& width, const UINT& height);
	void			RecreateGBufferViews(const UINT& width, const UINT& height);
	void			CreateDepthStencilView();
	void			CreateBackBufferView();
	void			CreateRTVForTexture(const ITexture2D* texture, wrl::ComPtr<ID3D11RenderTargetView>& rtv);

	// ImGuiStuff
	void			ShowRenderWindow(ID3D11ShaderResourceView* srv, bool* p_open = (bool*)0);
	bool			IsRenderingToImGui	= false;
	bool			ImGuiEnabled		= false;


	wrl::ComPtr<ID3D11Device>			p_Device;
	wrl::ComPtr<ID3D11DeviceContext>	p_Context;
	wrl::ComPtr<IDXGISwapChain>			p_SwapChain;
	wrl::ComPtr<ID3D11RasterizerState>  p_RSState;
	wrl::ComPtr<ID3D11BlendState>		p_BlendState;

	wrl::ComPtr<ID3D11RenderTargetView>		g_mainRenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView>		g_mainDepthStencilView;

private:
	// G-buffer
	wrl::ComPtr<ID3D11RenderTargetView>		rtvPosition;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvNormal;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvAlbedo;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvSpecular;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvLight;

	std::unique_ptr<RenderTexture>			PositionTexture;
	std::unique_ptr<RenderTexture>			NormalTexture;
	std::unique_ptr<RenderTexture>			AlbedoTexture;
	std::unique_ptr<RenderTexture>			SpecularTexture;
	std::unique_ptr<RenderTexture>			LightTexture;

	std::unique_ptr<PixelShaderCommon>										   pCombinePS;
	std::unique_ptr<PixelShaderCommon>										   pLightPassPixelShader;
	std::unique_ptr<VertexShaderCommon>										   pScreenSpaceVS;
	std::unique_ptr<Sampler>												   pLinearSampler;
	std::unique_ptr<PixelConstantBuffer<dx::XMFLOAT4>>						   pPixelCameraBuffer;

	// Material system
	MaterialSystem materialSystem;
	class DefferedRendering
	{
		// Maybe put all the deffered rendering stuff here
	};
};
