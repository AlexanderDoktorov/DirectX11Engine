#pragma once
#include <DirectXMath.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include "DirectXWindow.h"
#include "Camera.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

class Sampler;
class VertexShaderCommon;
class PixelShaderCommon;
class RenderTexture;

template <class T, size_t numStructs>
class StructuredBufferVS;

template <class T>
class PixelConstantBuffer;

interface ITexture;

class Graphics
{
	friend class GraphicsChild;
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

	// Render targets
	wrl::ComPtr<ID3D11ShaderResourceView> MakeSRVFromRTV(wrl::ComPtr<ID3D11RenderTargetView> rtv);

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
	void			CreateRTVForTexture(const ITexture* texture, wrl::ComPtr<ID3D11RenderTargetView>& rtv);

	// ImGuiStuff
	void			ShowRenderWindow(ID3D11ShaderResourceView* srv, bool* p_open = (bool*)0);
	bool			IsRenderingToImGui = false;
	bool			ImGuiEnabled = false;


	wrl::ComPtr<ID3D11Device>			p_Device;
	wrl::ComPtr<ID3D11DeviceContext>	p_Context;
	wrl::ComPtr<IDXGISwapChain>			p_SwapChain;
	wrl::ComPtr<ID3D11RasterizerState>  p_RSState;
	wrl::ComPtr<ID3D11BlendState>		p_BlendState;

	wrl::ComPtr<ID3D11RenderTargetView>		g_mainRenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView>		g_mainDepthStencilView;

	// G-buffer
	wrl::ComPtr<ID3D11RenderTargetView>		rtvPosition;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvNormal;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvAlbedo;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvLight;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvMaterialID;

	std::unique_ptr<RenderTexture>			PositionTexture;
	std::unique_ptr<RenderTexture>			NormalTexture;
	std::unique_ptr<RenderTexture>			AlbedoTexture;
	std::unique_ptr<RenderTexture>			LightTexture;
	std::unique_ptr<RenderTexture>			MaterialIDTexture;

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
