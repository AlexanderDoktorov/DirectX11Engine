#pragma once
#include <DirectXMath.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include "DirectXWindow.h"
#include "Camera.h"

#include "ImGui\backend\imgui_impl_dx11.h"
#include "ImGui\backend\imgui_impl_win32.h"
#include "ImGui\imgui.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

class VertexShaderCommon;
class PixelShaderCommon;
class Texture;
class Sampler;

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
	void			BindBackBuffer();
	void			MakeBackBufferTexture();

	Camera			GetCamera()		const;
	dx::XMMATRIX	GetProjection() const noexcept;

	~Graphics();

private:
	dx::XMMATRIX	projection		= dx::XMMatrixIdentity();
	Camera			cam				= Camera();

	void			RecreateMainViews(const UINT& width, const UINT& height);
	void			RecreateGBufferViews(const UINT& width, const UINT& height);
	void			CreateDepthStencilView();
	void			CreateBackBufferView();
	void			CreateRTVForTexture(const Texture& texture, wrl::ComPtr<ID3D11RenderTargetView>& rtv);
	void			UnbindRenderTargets(UINT num_views);
	void			UnbindPixelShaderResourses(UINT num_resourses);
	void			ClearRTV(ID3D11RenderTargetView* rtv, const float clear_color[4]);


	// ImGuiStuff
	void			ShowRenderWindow(bool* p_open = (bool*)0);
	bool			IsRenderingToImGui = false;
	bool			ImGuiEnabled = false;


	wrl::ComPtr<ID3D11Device>			p_Device;
	wrl::ComPtr<ID3D11DeviceContext>	p_Context;
	wrl::ComPtr<IDXGISwapChain>			p_SwapChain;
	wrl::ComPtr<ID3D11RasterizerState>  p_RSState;

	wrl::ComPtr<ID3D11RenderTargetView>		g_mainRenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView>		g_mainDepthStencilView;
	wrl::ComPtr<ID3D11ShaderResourceView>	g_backBufferTextureView;

	// G-buffer
	wrl::ComPtr<ID3D11RenderTargetView>		rtvPosition;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvNormal;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvAlbedo;
	wrl::ComPtr<ID3D11RenderTargetView>		rtvLight;

	std::unique_ptr<Texture>			PositionTexture;
	std::unique_ptr<Texture>			NormalTexture;
	std::unique_ptr<Texture>			AlbedoTexture;
	std::unique_ptr<Texture>			LightTexture;

	std::unique_ptr<PixelShaderCommon>			 pCombinePS;
	std::unique_ptr<PixelShaderCommon>			 pLightPassPixelShader;
	std::unique_ptr<VertexShaderCommon>			 pScreenSpaceVS;
	std::unique_ptr<Sampler>					 pLinearSampler;
	class DefferedRendering
	{
		// Maybe put all the deffered rendering stuff here
	};
};
