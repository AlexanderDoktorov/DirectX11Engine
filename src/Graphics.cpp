#include "Graphics.h"
#include "hrException.h"
#include "DOK_DX11.h"
#include <algorithm>
#include <assert.h>
#include "imgui_impl_dx11.h"
#include "SlotLayout.h"
#include "DirectXWindow.h"
#include "RenderTexture.h"
#include "Sampler.h"
#include "PixelShaderCommon.h"
#include "VertexShaderCommon.h"
#include "PixelConstantBuffer.h"
#include "Light.h"

#pragma comment(lib, "d3d11")

Graphics::Graphics(HWND hwnd)
{
    // Set flags
    UINT device_flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    // Fill feature levels descriptor struct
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
    swap_chain_desc.BufferCount = 2;
    swap_chain_desc.BufferDesc.Width = 0;
    swap_chain_desc.BufferDesc.Height = 0;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swap_chain_desc.OutputWindow = hwnd;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    HRESULT hr;
    // Create Device And Swap Chain
    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        device_flags,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &swap_chain_desc,
        &p_SwapChain,
        &p_Device,
        &featureLevel,
        &p_Context
    );
    if (hr == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, device_flags, featureLevelArray, 2, D3D11_SDK_VERSION, &swap_chain_desc, &p_SwapChain, &p_Device, &featureLevel, &p_Context);
    CHECK_HR(hr);

    assert(S_OK == hr && p_SwapChain && p_Device && p_Context);

    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC DSD = {};
    DSD.DepthEnable = TRUE;
    DSD.DepthFunc = D3D11_COMPARISON_LESS; // means that object is overwritten when it's Z is closer - LOGIC
    DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    wrl::ComPtr<ID3D11DepthStencilState> p_DSState;
    hr = p_Device->CreateDepthStencilState(&DSD, &p_DSState); CHECK_HR(hr);

    // Bind depth stencil state
    p_Context->OMSetDepthStencilState(p_DSState.Get(), 1U);
    CreateBackBufferView();
    CreateDepthStencilView();

    RECT rc;
    GetClientRect(hwnd, &rc);
    D3D11_VIEWPORT initial_viewport = D3D11_VIEWPORT{
        0.f,
        0.f,
        (FLOAT)(rc.right - rc.left),
        (FLOAT)(rc.bottom - rc.top),
        0.f,
        1.f
    };
    p_Context->RSSetViewports(1U, &initial_viewport);

    // Set RSState
    D3D11_RASTERIZER_DESC RSDesc{};
    RSDesc.CullMode = D3D11_CULL_BACK;
    RSDesc.FillMode = D3D11_FILL_SOLID;

    p_Device->CreateRasterizerState(&RSDesc, &p_RSState);
    p_Context->RSSetState(p_RSState.Get());

    // Create G-Buffer
    defferedRenderer.Initilize(*this, rc);

    p_SceneBuffer = std::make_unique<scene_buffer_type>(*this, SceneBuffer{}, SLOT_BUFFER_SCENE);

    ImGui_ImplDX11_Init(p_Device.Get(), p_Context.Get());
}

void Graphics::BeginFrame(const window_type* pWnd, std::function<void()> _FGeomPass)
{
    ResizeInfo resInfo = pWnd->ResetResizeInfo();
    if (resInfo.g_ResizeWidth != 0 && resInfo.g_ResizeHeight != 0) {
        if (rendererType == RENDERER_TYPE_DEFFERED) {
            defferedRenderer.OnResize(*this, pWnd->GetWndRect());
        }
        OnResize(pWnd->GetWndRect());
    }

    if (ImGuiEnabled)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    if (IsRenderingToImGui)
        ImGui::DockSpaceOverViewport();

    p_SceneBuffer->Update(*this, SceneBuffer(static_cast<uint32_t>(m_Lights.size()), camera.GetPos()));
    p_SceneBuffer->Bind(*this);

    switch (rendererType)
    {
    case RENDERER_TYPE_DEFFERED:
    {
        // ↓↓↓↓ GEOMETRY PASS ↓↓↓↓
        defferedRenderer.BeginGeometryPass(*this);
        {
            if(_FGeomPass)
                _FGeomPass();
            std::for_each(m_Lights.begin(), m_Lights.end(), [this](std::unique_ptr<Light>& lightSource) {
                if (Drawable* drawableLightSource = dynamic_cast<Drawable*>(lightSource.get()))
                    drawableLightSource->Draw(*this);
            });
        }
        defferedRenderer.EndGeometryPass(*this);
        // ↓↓↓↓ LIGHT PASS ↓↓↓↓
        defferedRenderer.BeginLightPass(*this);
        {
            std::for_each(m_Lights.begin(), m_Lights.end(), [this](std::unique_ptr<Light>& lightSource) {
                lightSource->Bind(*this);
                Draw(3U);
            });
        }
        defferedRenderer.EndLightPass(*this);
        // ↓↓↓↓ COMINE PASS ↓↓↓↓
        defferedRenderer.PerformCombinePass(*this, g_mainRenderTargetView.GetAddressOf());
    }
    break;
    case RENDERER_TYPE_FORWARD:
    {
        const float clear_color[4] = { 0.f,0.f,0.f,1.f };
        BeginForwardFrame(pWnd, clear_color);
    }
    break;
    [[unlikely]] 
    case RENDERER_TYPE_MIXED:
        break;
    default:
        break;
    }

}

void Graphics::ShowRenderWindow(ID3D11ShaderResourceView* srv, bool* p_open)
{
    if ((ImGui::Begin("Viewport", p_open)))
    {
        ImVec2 client_region_size_with_indent = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
        ImGui::GetWindowDrawList()->AddImage(srv, ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + client_region_size_with_indent);
    } 
    ImGui::End();
}

void Graphics::BeginForwardFrame(const window_type* pWnd, const float clear_color[4])
{
    p_Context->ClearDepthStencilView(g_mainDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0U);
    p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), g_mainDepthStencilView.Get());
    p_Context->ClearRenderTargetView(g_mainRenderTargetView.Get(), clear_color);
}

void Graphics::AddLightSource(std::unique_ptr<Light> p_Light)
{
    m_Lights.push_back(std::move(p_Light));
}

void Graphics::EndFrame()
{
    wrl::ComPtr<ID3D11ShaderResourceView> backBuffer{};

    if (IsRenderingToImGui)
    {
        // First make SRV then clear RTV !!!
        backBuffer = MakeSRVFromRTV(g_mainRenderTargetView);
        const float clear_color[4] = { 0.2f, 0.2f, 0.2f , 0.1f };
        p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), g_mainDepthStencilView.Get());
        p_Context->ClearRenderTargetView(g_mainRenderTargetView.Get(), clear_color);

        static bool open = true;
        if (backBuffer)
            ShowRenderWindow(backBuffer.Get(), &open);
    }
    else
        p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), g_mainDepthStencilView.Get());

    if (ImGuiEnabled)
    {
        ImGui::Render();
        ImGui::EndFrame();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    HRESULT hr;
    if (FAILED(hr = p_SwapChain->Present(1U, 0U)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
            throw hrException(__LINE__, __FILE__, p_Device->GetDeviceRemovedReason());
        else
            throw hrException(__LINE__, __FILE__, hr);
    }
}

void Graphics::DrawIndexed(UINT index_count)
{
    p_Context->DrawIndexed(index_count, 0U, 0U);
}

void Graphics::Draw(UINT vertex_count)
{
    p_Context->Draw(vertex_count, 0U);
}

void Graphics::RenderToImGui(const bool& state)
{
    IsRenderingToImGui = state;
}

Graphics::~Graphics()
{
    ImGui_ImplDX11_Shutdown();
}

void Graphics::ResizeViews(const UINT& width, const UINT& height)
{
    g_mainDepthStencilView.Reset();
    g_mainRenderTargetView.Reset();
    p_SwapChain->ResizeBuffers(0U, width, height, DXGI_FORMAT_UNKNOWN, 0);
    CreateDepthStencilView();
    CreateBackBufferView();
}

void Graphics::CreateDepthStencilView()
{
    HRESULT hr;
    ID3D11Texture2D* pBackBuffer;
    hr = p_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)); CHECK_HR(hr);

    D3D11_TEXTURE2D_DESC BackBufferDesc;
    pBackBuffer->GetDesc(&BackBufferDesc);

    wrl::ComPtr<ID3D11Texture2D> pDepthStencil = {};
    D3D11_TEXTURE2D_DESC DSTextureDesc = {};
    DSTextureDesc.Height = BackBufferDesc.Height;
    DSTextureDesc.Width = BackBufferDesc.Width;
    DSTextureDesc.MipLevels = 1U;
    DSTextureDesc.ArraySize = 1U;
    DSTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSTextureDesc.SampleDesc.Count = 1U;
    DSTextureDesc.SampleDesc.Quality = 0U;
    DSTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DSTextureDesc.Usage = D3D11_USAGE_DEFAULT;

    hr = p_Device->CreateTexture2D(&DSTextureDesc, nullptr, &pDepthStencil); CHECK_HR(hr);

    // Create view (same as we did with "View" on back buffer with render target view)
    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0U;

    hr = p_Device->CreateDepthStencilView(pDepthStencil.Get(), &DSVDesc, &g_mainDepthStencilView); CHECK_HR(hr);
    pBackBuffer->Release();
}

void Graphics::CreateBackBufferView()
{
    HRESULT hr;
    ID3D11Texture2D* pBackBuffer;
    hr = p_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)); CHECK_HR(hr);
    hr = p_Device->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView); CHECK_HR(hr);
    pBackBuffer->Release();
}

void Graphics::CreateRTVForTexture(const ITexture2D* texture, wrl::ComPtr<ID3D11RenderTargetView>& rtv)
{
    assert(texture->GetDesc().BindFlags | D3D11_BIND_RENDER_TARGET);

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = texture->GetDesc().Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0U;
    HRESULT hr = p_Device->CreateRenderTargetView(texture->GetTexture(), &rtvDesc, &rtv); CHECK_HR(hr);
}

wrl::ComPtr<ID3D11ShaderResourceView> Graphics::MakeSRVFromRTV(wrl::ComPtr<ID3D11RenderTargetView> rtv)
{
    ID3D11Texture2D* pRenderTextureCopy = nullptr;
    ID3D11Texture2D* pRenderTexture = nullptr;
    rtv->GetResource(reinterpret_cast<ID3D11Resource**>(&pRenderTexture));
    if (!pRenderTexture)
        return nullptr;

    D3D11_TEXTURE2D_DESC descText{};
    pRenderTexture->GetDesc(&descText);
    descText.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    descText.MipLevels = 0U;
    descText.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    HRESULT hr = p_Device->CreateTexture2D(&descText, nullptr, &pRenderTextureCopy);
    if (FAILED(hr))
        return nullptr;

    // because backbuffer haven't mipmap, we couldn't copy its texture just with 'CopyResource()', so we're update only most detailed mip-level
    p_Context->CopySubresourceRegion(pRenderTextureCopy, 0U, 0U, 0U, 0U, pRenderTexture, 0U, nullptr);

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDescBuffer{};
    resourceViewDescBuffer.Format = descText.Format;
    resourceViewDescBuffer.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDescBuffer.Texture2D.MostDetailedMip = 0U;
    resourceViewDescBuffer.Texture2D.MipLevels = ~0U;

    wrl::ComPtr<ID3D11ShaderResourceView> srv;
    hr = p_Device->CreateShaderResourceView(pRenderTextureCopy, &resourceViewDescBuffer, &srv);
    if (FAILED(hr))
        return nullptr;

    // regenerate mipmap based on updated most detailed mip-level
    p_Context->GenerateMips(srv.Get());

    pRenderTextureCopy->Release();
    pRenderTexture->Release();

    return srv;
}

wrl::ComPtr<ID3D11RenderTargetView> Graphics::MakeRTVFromTexture(ID3D11Device* p_Device, const ITexture2D* texture)
{
    assert(texture->GetDesc().BindFlags | D3D11_BIND_RENDER_TARGET);

    wrl::ComPtr<ID3D11RenderTargetView> rtv;

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = texture->GetDesc().Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0U;
    CHECK_EXPR_DEFINE_HR(p_Device->CreateRenderTargetView(texture->GetTexture(), &rtvDesc, &rtv));

    return rtv;
}

void Graphics::OnResize(const RECT& winRect)
{
    long width = winRect.right - winRect.left;
    long height = winRect.bottom - winRect.top;
    ResizeViews(width, height);
    D3D11_VIEWPORT viewport = D3D11_VIEWPORT{
        (FLOAT)winRect.left,
        (FLOAT)winRect.top,
        (FLOAT)(width),
        (FLOAT)(height),
        0.f,
        1.f
    };
    p_Context->RSSetViewports(1U, &viewport);
}

void Graphics::SetProjection(dx::XMMATRIX projection) noexcept
{
    this->projectionMatrix = projection;
}

void Graphics::SetRendererType(RENDERER_TYPE rt) noexcept
{
    rendererType = rt;
}

dx::XMMATRIX Graphics::GetProjection() const noexcept
{
    return projectionMatrix;
}

RENDERER_TYPE Graphics::GetRendererType() const noexcept
{
    return rendererType;
}

void Graphics::SetCamera(const Camera& new_cam)
{
    camera = new_cam;
}

void Graphics::SetAdditiveBlendingState()
{
    D3D11_BLEND_DESC addBlendDesc{};
    addBlendDesc.AlphaToCoverageEnable                 = FALSE;
    addBlendDesc.IndependentBlendEnable                = FALSE;
    addBlendDesc.RenderTarget[0].BlendEnable           = TRUE;
    addBlendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
    addBlendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    addBlendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (!p_BlendState.Get())
    {
        HRESULT hr = p_Device->CreateBlendState(&addBlendDesc, &p_BlendState);
        CHECK_HR(hr);
    }
    else
    {
        D3D11_BLEND_DESC currentBlendDesc{};
        p_BlendState->GetDesc(&currentBlendDesc);

        if (currentBlendDesc != addBlendDesc)
        {
            HRESULT hr = p_Device->CreateBlendState(&addBlendDesc, &p_BlendState);
            CHECK_HR(hr);
        }
    }
    
    p_Context->OMSetBlendState(p_BlendState.Get(), nullptr, UINT_MAX);
}

void Graphics::ResetBlendingState() noexcept
{
    p_Context->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}

Camera Graphics::GetCamera() const
{
    return camera;
}

                                                                                        /************ DefferedRenderer ************/
                                                                                        /*||||||||||| DefferedRenderer |||||||||||*/
                                                                                        /*VVVVVVVVVVV DefferedRenderer VVVVVVVVVVV*/

void Graphics::DefferedRenderer::Initilize(Graphics& Gfx, const RECT& rc)
{
    // Create textures
    PositionTexture         = std::make_unique<RenderTexture>(Gfx, DXGI_FORMAT_R16G16B16A16_FLOAT, rc.bottom - rc.top, rc.right - rc.left);
    NormalTexture           = std::make_unique<RenderTexture>(Gfx, DXGI_FORMAT_R16G16B16A16_FLOAT, rc.bottom - rc.top, rc.right - rc.left);
    AlbedoTexture           = std::make_unique<RenderTexture>(Gfx, DXGI_FORMAT_R8G8B8A8_UNORM,     rc.bottom - rc.top, rc.right - rc.left);
    SpecularTexture         = std::make_unique<RenderTexture>(Gfx, DXGI_FORMAT_R16G16B16A16_UNORM, rc.bottom - rc.top, rc.right - rc.left);
    LightTexture            = std::make_unique<RenderTexture>(Gfx, DXGI_FORMAT_R8G8B8A8_UNORM,     rc.bottom - rc.top, rc.right - rc.left);

    // Create render targets for textures
    Gfx.CreateRTVForTexture(PositionTexture.get(),   rtvPosition);
    Gfx.CreateRTVForTexture(NormalTexture.get(),     rtvNormal);
    Gfx.CreateRTVForTexture(AlbedoTexture.get(),     rtvAlbedo);
    Gfx.CreateRTVForTexture(SpecularTexture.get(),   rtvSpecular);
    Gfx.CreateRTVForTexture(LightTexture.get(),      rtvLight);

    // Create shaders
    pLightPassPixelShader   = std::make_unique<PixelShaderCommon>(Gfx,  L"shaders\\LightPS.cso");
    pScreenSpaceVS          = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\ScreenSpaceVS.cso");
    pCombinePS              = std::make_unique<PixelShaderCommon>(Gfx,  L"shaders\\CombinePS.cso");

    // Sampler
    p_Sampler = Sampler::Resolve(Gfx);
}

void Graphics::DefferedRenderer::OnResize(Graphics& Gfx, const RECT& winRect)
{
    auto width = winRect.right - winRect.left;
    auto height = winRect.bottom - winRect.top;
    PositionTexture->Resize(Gfx, height, width);
    NormalTexture->Resize(Gfx, height, width);
    AlbedoTexture->Resize(Gfx, height, width);
    LightTexture->Resize(Gfx, height, width);
    SpecularTexture->Resize(Gfx, height, width);

    Gfx.CreateRTVForTexture(PositionTexture.get(), rtvPosition);
    Gfx.CreateRTVForTexture(NormalTexture.get(), rtvNormal);
    Gfx.CreateRTVForTexture(AlbedoTexture.get(), rtvAlbedo);
    Gfx.CreateRTVForTexture(LightTexture.get(), rtvLight);
    Gfx.CreateRTVForTexture(SpecularTexture.get(), rtvSpecular);
}
void Graphics::DefferedRenderer::PerformCombinePass(Graphics& Gfx, ID3D11RenderTargetView** outputRtv) const
{
    auto& p_Context = Gfx.p_Context;
    const float colorClear[4] = { 0.f,0.f,0.f,1.f };
    p_Context->ClearRenderTargetView( *(outputRtv), colorClear);
    p_Context->IASetInputLayout(nullptr);

    ID3D11ShaderResourceView* srvs[2] = { AlbedoTexture->GetSRV(), LightTexture->GetSRV() };

    p_Context->PSSetShaderResources(0U, ARRAYSIZE(srvs), srvs);
    p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    p_Context->OMSetRenderTargets(1U, outputRtv, nullptr);
    pCombinePS->Bind(Gfx);
    pScreenSpaceVS->Bind(Gfx);
    p_Sampler->Bind(Gfx);
    Gfx.Draw(3U);

    // Unbind shader resourses
    ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
    p_Context->PSSetShaderResources(0U, 2U, nullSRV);

    // Unbind render targets
    ID3D11RenderTargetView* nullRTV = nullptr;
    p_Context->OMSetRenderTargets(1U, &nullRTV, nullptr);
}

void Graphics::DefferedRenderer::BeginLightPass(Graphics& Gfx) const
{
    auto& p_Context = Gfx.p_Context;
    // Clear render target before fill the light info
    const float light_clear[4] = { 0.f,0.f,0.f,0.f };
    p_Context->ClearRenderTargetView(rtvLight.Get(), light_clear);
    // Set additive blending state to sum up results from all lights
    Gfx.SetAdditiveBlendingState();

    // Bind shader resourses
    ID3D11ShaderResourceView* srvs[4] = { PositionTexture->GetSRV(), NormalTexture->GetSRV(), AlbedoTexture->GetSRV(), SpecularTexture->GetSRV() };
    p_Context->OMSetRenderTargets(1U, rtvLight.GetAddressOf(), nullptr);
    p_Context->PSSetShaderResources(0U , ARRAYSIZE(srvs), srvs);

    // Bind CameraBuffer
    //Gfx.BindCameraBuffer();

    // Bind shaders
    pScreenSpaceVS->Bind(Gfx);
    pLightPassPixelShader->Bind(Gfx);

    // Bind linear sampler
    p_Sampler->Bind(Gfx);
}

void Graphics::DefferedRenderer::EndLightPass(Graphics& Gfx) const
{
    auto& p_Context = Gfx.p_Context;
    // Unbind resourses
    ID3D11ShaderResourceView* nullSRVs[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
    p_Context->PSSetShaderResources(0U, ARRAYSIZE(nullSRVs), nullSRVs);

    // Unbind render targets
    ID3D11RenderTargetView* nullRTV = nullptr;
    p_Context->OMSetRenderTargets(1U, &nullRTV, nullptr);

    Gfx.ResetBlendingState();
}

void Graphics::DefferedRenderer::BeginGeometryPass(Graphics& Gfx) noexcept
{
    ID3D11RenderTargetView* rtvs[4] = { rtvPosition.Get(), rtvNormal.Get(), rtvAlbedo.Get(), rtvSpecular.Get() };

    // Clear render targets
    const float rtvClear [4] = { 0.f,0.f,0.f,0.f };
    for (size_t i = 0; i < ARRAYSIZE(rtvs); i++) {
        Gfx.p_Context->ClearRenderTargetView(rtvs[i], rtvClear);
    }

    Gfx.p_Context->ClearDepthStencilView(Gfx.g_mainDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0U);
    Gfx.p_Context->OMSetRenderTargets(ARRAYSIZE(rtvs), rtvs, Gfx.g_mainDepthStencilView.Get());
}

void Graphics::DefferedRenderer::EndGeometryPass(Graphics& Gfx) const noexcept
{
    ID3D11RenderTargetView* nullRTVs[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
    Gfx.p_Context->OMSetRenderTargets(ARRAYSIZE(nullRTVs), nullRTVs, nullptr);
}
