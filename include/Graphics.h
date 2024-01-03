#pragma once
#include <DirectXMath.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include "material.h"
#include "DirectXWindow.h"
#include "Camera.h"

#include "imgui_impl_dx11.h"

#include "IBindable.h"
#include "VertexShaderCommon.h"
#include "PixelShaderCommon.h"
#include "PixelConstantBuffer.h"
#include "RenderTexture.h"
#include "Interfaces.h"
#include "Sampler.h"
#include "MaterialSystem.h"
#include "noxnd.h"
#include "DOK_traits.h"

namespace wrl = Microsoft::WRL;
namespace dx  = DirectX;

#define MAX_MATERIALS (size_t)100

class Graphics
{
public:
	// Meta info
	using window_type = DirectXWindow;
private:
	friend class GraphicsChild;
public:
	Graphics(HWND hwnd);

	void BeginFrame(const DirectXWindow* pWnd, const float clear_color[4]);

	// Deffered Rendering
	void			ResizeRenderTargetViews(const DirectXWindow* pWnd);

	void			BeginGeometryPass(const DirectXWindow* pWnd);
	void			EndGeometryPass() noexcept;
	void			BeginLightningPass();
	void			EndLightningPass() noexcept;
	void			PerformCombinePass();

	void			SetProjection(dx::XMMATRIX projection) noexcept;
	void			SetCamera(const Camera& cam);

	void			SetAdditiveBlendingState();
	void			ResetBlendingState();
	void			EndFrame();
	void			DrawIndexed(UINT Count);
	void			Draw(UINT vertex_count);
	void			RenderToImGui(const bool& state);

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

	class DefferedRendering
	{
		// Maybe put all the deffered rendering stuff here
	};
};
