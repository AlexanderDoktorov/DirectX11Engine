#pragma once
#include <DirectXMath.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include "material.h"
#include "Camera.h"

#include "noxnd.h"
#include "DOK_traits.h"

namespace wrl = Microsoft::WRL;
namespace dx  = DirectX;

#pragma region forward_declarations
class DirectXWindow;
class RenderTexture;
class Sampler;
class PixelShaderCommon;
class VertexShaderCommon;
class Light;
template <class T> class PixelConstantBuffer;
#pragma endregion forward_declarations

enum RENDERER_TYPE
{
	RENDERER_TYPE_DEFFERED,
	RENDERER_TYPE_FORWARD,
	RENDERER_TYPE_MIXED // unused
};

class Graphics
{
	struct SceneBuffer
	{
		uint32_t	 numLights{};
		dx::XMFLOAT3 worldCameraPos{};
	};
	using window_type		= DirectXWindow;
	using scene_buffer_type = PixelConstantBuffer<SceneBuffer>;
	friend class GraphicsChild;
public:
	Graphics(HWND hwnd);
	void OnResize(const RECT& winRect);

	void BeginFrame(const window_type* pWnd, std::function<void()> _FGeomPass = nullptr);
	void BeginForwardFrame(const window_type* pWnd, const float clear_color[4]);
	void AddLightSource(std::unique_ptr<Light> p_Light);
	void DrawIndexed(UINT index_count);
	void Draw(UINT vertex_count);
	void EndFrame();
	void ResetBlendingState() noexcept;
	void RenderToImGui(const bool& state);
	void SetProjection(dx::XMMATRIX projection) noexcept;
	void SetRendererType(RENDERER_TYPE rt) noexcept;
	void SetCamera(const Camera& cam);
	void SetAdditiveBlendingState();

	Camera		  GetCamera() const;
	dx::XMMATRIX  GetProjection() const noexcept;
	RENDERER_TYPE GetRendererType() const noexcept;

	// Render targets
	wrl::ComPtr<ID3D11ShaderResourceView> MakeSRVFromRTV(wrl::ComPtr<ID3D11RenderTargetView> rtv);
	static wrl::ComPtr<ID3D11RenderTargetView> MakeRTVFromTexture(ID3D11Device* p_Device, const ITexture2D* texture);

	~Graphics();
private:
	void ResizeViews(const UINT& width, const UINT& height);
	void CreateDepthStencilView();
	void CreateBackBufferView();
	void CreateRTVForTexture(const ITexture2D* texture, wrl::ComPtr<ID3D11RenderTargetView>& rtv);
	void ShowRenderWindow(ID3D11ShaderResourceView* srv, bool* p_open = (bool*)0);

	RENDERER_TYPE	rendererType		= RENDERER_TYPE_FORWARD;
	dx::XMMATRIX	projectionMatrix	= dx::XMMatrixIdentity();
	Camera			camera				= Camera();
	bool			IsRenderingToImGui	= true;
	bool			ImGuiEnabled		= true;

	wrl::ComPtr<ID3D11Device>			p_Device;
	wrl::ComPtr<ID3D11DeviceContext>	p_Context;
	wrl::ComPtr<IDXGISwapChain>			p_SwapChain;
	wrl::ComPtr<ID3D11RasterizerState>  p_RSState;
	wrl::ComPtr<ID3D11BlendState>		p_BlendState;
	std::unique_ptr<scene_buffer_type>	p_SceneBuffer;
	std::vector<std::unique_ptr<Light>>	m_Lights;

	wrl::ComPtr<ID3D11RenderTargetView>		g_mainRenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView>		g_mainDepthStencilView;

	struct DefferedRenderer
	{
		DefferedRenderer() = default;
		void Initilize(Graphics& Gfx, const RECT& rc);
		void OnResize(Graphics& Gfx, const RECT& winRect);
		// Passes
		void PerformCombinePass(Graphics& Gfx, ID3D11RenderTargetView** outputRtv) const;
		void BeginLightPass(Graphics& Gfx) const;
		void EndLightPass(Graphics& Gfx) const;
		void BeginGeometryPass(Graphics& Gfx) noexcept;
		void EndGeometryPass(Graphics& Gfx) const noexcept;

		// Shaders
		std::unique_ptr<PixelShaderCommon>		pCombinePS;
		std::unique_ptr<PixelShaderCommon>		pLightPassPixelShader;
		std::unique_ptr<VertexShaderCommon>		pScreenSpaceVS;

		// Render targets
		wrl::ComPtr<ID3D11RenderTargetView>		rtvPosition;
		wrl::ComPtr<ID3D11RenderTargetView>		rtvNormal;
		wrl::ComPtr<ID3D11RenderTargetView>		rtvAlbedo;
		wrl::ComPtr<ID3D11RenderTargetView>		rtvSpecular;
		wrl::ComPtr<ID3D11RenderTargetView>		rtvLight;

		// Textures
		std::unique_ptr<RenderTexture>			PositionTexture;
		std::unique_ptr<RenderTexture>			NormalTexture;
		std::unique_ptr<RenderTexture>			AlbedoTexture;
		std::unique_ptr<RenderTexture>			SpecularTexture;
		std::unique_ptr<RenderTexture>			LightTexture;

		// Sampler
		std::shared_ptr<Sampler> p_Sampler;
	} defferedRenderer;
};
