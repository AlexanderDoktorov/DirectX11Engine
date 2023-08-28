#pragma once
#include <DirectXMath.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3d11.h>
#include "ResizingBaseWindow.h"
#include "Camera.h"

#include "ImGui\backend\imgui_impl_dx11.h"
#include "ImGui\backend\imgui_impl_win32.h"
#include "ImGui\imgui.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

class Graphics
{
	friend class IBindable;
public:
	Graphics(HWND hwnd);

	template<class T>
	void BeginFrame(const ResizingBaseWindow<T>* pWnd, const float clear_color[4])
	{
		ResizeViews(pWnd);

		if (ImGuiEnabled)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}

		if (IsRenderingToImGui)
		{
			ImGui::DockSpaceOverViewport();
		}

		ClearRenderTarget(clear_color);
		p_Context->ClearDepthStencilView(g_mainDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0U);
		p_Context->RSSetViewports(1U, &vp);
		p_Context->RSSetState(p_RSState.Get());
		p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), g_mainDepthStencilView.Get());
	}

	void			SetProjection(dx::XMMATRIX projection) noexcept;
	void			SetCamera(const Camera& cam);
	void			ClearRenderTarget(const float clear_color[4]);
	void			EndFrame();
	void			DrawIndexed(UINT Count);
	void			RenderToImGui(const bool& state);

	Camera			GetCamera()		const;
	dx::XMMATRIX	GetProjection() const noexcept;

	~Graphics();

private:
	dx::XMMATRIX	projection		= dx::XMMatrixIdentity();
	Camera			cam				= Camera();
	D3D11_VIEWPORT  vp;

	void			ResizeBackBuffer(const UINT& width, const UINT& height);
	void			CreateDepthStencilView();
	void			CreateRenderTargetView();

	// ImGuiStuff
	void			ShowRenderWindow(bool* p_open = (bool*)0);
	void			SetBackBufferAsShaderResourse();
	bool			IsRenderingToImGui = false;
	bool			ImGuiEnabled = true;

	template<class T>
	void ResizeViews(const ResizingBaseWindow<T>* pWnd)
	{
		if (pWnd->GetResizeInfo().g_ResizeWidth != 0 && pWnd->GetResizeInfo().g_ResizeHeight != 0)
		{
			ResizeBackBuffer(pWnd->GetResizeInfo().g_ResizeWidth, pWnd->GetResizeInfo().g_ResizeHeight);
			pWnd->ZeroResizeInfo();
			RECT rc;
			GetClientRect(pWnd->GetWnd(), &rc);
			vp = D3D11_VIEWPORT{
				(FLOAT)rc.left,
				(FLOAT)rc.top,
				(FLOAT)(rc.right - rc.left),
				(FLOAT)(rc.bottom - rc.top),
				0.f,
				1.f
			};
		}
	}
	wrl::ComPtr<ID3D11Device>			p_Device;
	wrl::ComPtr<ID3D11DeviceContext>	p_Context;
	wrl::ComPtr<IDXGISwapChain>			p_SwapChain;
	wrl::ComPtr<ID3D11RasterizerState>  p_RSState;

	wrl::ComPtr<ID3D11RenderTargetView>		g_mainRenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView>		g_mainDepthStencilView;
	wrl::ComPtr<ID3D11ShaderResourceView>	g_mainShaderResourseView;
};