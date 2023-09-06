#include "Graphics.h"
#include "Exceptions.h"
#include <assert.h>

#include "Texture.h"
#include "LightPassPixelShader.h"
#include "ScreenSpaceVertexShader.h"
#include "CombinePixelShader.h"

#pragma comment(lib, "d3d11")

Graphics::Graphics(HWND hwnd) : 
    projection(dx::XMMATRIX()),
    ImGuiEnabled(true)
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
    ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
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

    // Create depth stencil state (to next create texture)
    D3D11_DEPTH_STENCIL_DESC DSD = {};
    DSD.DepthEnable = TRUE;
    DSD.DepthFunc = D3D11_COMPARISON_LESS; // means that object is overwritten when it's Z is closer - LOGIC
    DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    wrl::ComPtr<ID3D11DepthStencilState> p_DSState;
    CHECK_HR( p_Device->CreateDepthStencilState(&DSD, &p_DSState) );

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
    PositionTexture         = std::make_unique<Texture>(*this, DXGI_FORMAT_R16G16B16A16_FLOAT, rc.bottom - rc.top, rc.right - rc.left);
    NormalTexture           = std::make_unique<Texture>(*this, DXGI_FORMAT_R16G16B16A16_FLOAT, rc.bottom - rc.top, rc.right - rc.left);
    AlbedoTexture           = std::make_unique<Texture>(*this, DXGI_FORMAT_R8G8B8A8_UNORM, rc.bottom - rc.top, rc.right - rc.left);
    LightTexture            = std::make_unique<Texture>(*this, DXGI_FORMAT_R8G8B8A8_UNORM, rc.bottom - rc.top, rc.right - rc.left);
    
    CreateRTVForTexture(*PositionTexture,   rtvPosition);
    CreateRTVForTexture(*NormalTexture,     rtvNormal);
    CreateRTVForTexture(*AlbedoTexture,     rtvAlbedo);
    CreateRTVForTexture(*LightTexture,      rtvLight);

    pLightPassPixelShader   = std::make_unique<LightPassPixelShader>(*this);
    pScreenSpaceVS = std::make_unique<ScreenSpaceVertexShader>(*this);
    pCombinePS = std::make_unique<CombinePixelShader>(*this);

    ImGui_ImplDX11_Init(p_Device.Get(), p_Context.Get());
}

void Graphics::BindBackBuffer()
{
    p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), g_mainDepthStencilView.Get());
}

void Graphics::BeginGeometryPass(const DirectXWindow* pWnd, const float clear_color[4])
{
    ResizeRenderTargetViews(pWnd);

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

    ID3D11RenderTargetView* rtvs[3] = { rtvPosition.Get(), rtvNormal.Get(), rtvAlbedo.Get() };
    for (int i = 0; i < 3; ++i) {
       ClearRTV(rtvs[i], clear_color);
    }
    p_Context->OMSetRenderTargets(3U, rtvs, nullptr);
}

void Graphics::EndGeometryPass()
{
    ID3D11RenderTargetView* nullRTVs[3] = { nullptr, nullptr, nullptr };
    p_Context->OMSetRenderTargets(3U, nullRTVs, nullptr);
}

void Graphics::BeginLightningPass()
{
    const float light_clear[4] = { 0.f,0.f,0.f,0.f };
    ClearRTV(rtvLight.Get(), light_clear);
    SetAdditiveBlendingState();

    ID3D11ShaderResourceView* srvs[3] = { PositionTexture->GetSRV(), NormalTexture->GetSRV(), AlbedoTexture->GetSRV() };
    p_Context->OMSetRenderTargets(1U, rtvLight.GetAddressOf(), nullptr);
    p_Context->PSSetShaderResources(0U , ARRAYSIZE(srvs), srvs);
    pScreenSpaceVS->Bind(*this);
    pLightPassPixelShader->Bind(*this);
}

void Graphics::EndLightningPass()
{
    ID3D11ShaderResourceView* nullSRVs[3] = { nullptr, nullptr, nullptr };
    p_Context->PSSetShaderResources(0U, ARRAYSIZE(nullSRVs), nullSRVs);

    ID3D11RenderTargetView* nullRTV = nullptr;
    p_Context->OMSetRenderTargets(1U, &nullRTV, nullptr);

    ResetBlendingState();
}

void Graphics::PerformCombinePass()
{
    p_Context->IASetInputLayout(nullptr);
    const float light_clear[4] = { 0.1f,0.1f,0.1f,0.3f };
    ClearRTV(g_mainRenderTargetView.Get(), light_clear);
    SetAdditiveBlendingState();

    ID3D11ShaderResourceView* srvs[4] = { PositionTexture->GetSRV(), NormalTexture->GetSRV(), AlbedoTexture->GetSRV(), LightTexture->GetSRV() };

    p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), nullptr);
    p_Context->PSSetShaderResources(0U, 4U, srvs);
    pCombinePS->Bind(*this);
    pScreenSpaceVS->Bind(*this);
    Draw(3U);

    // Unbind
    ID3D11ShaderResourceView* nullSRV[4] = { nullptr, nullptr, nullptr, nullptr };
    p_Context->PSSetShaderResources(0U, 4U, nullSRV);
    ResetBlendingState();
}

void Graphics::ShowRenderWindow(bool* p_open)
{
    if ((ImGui::Begin("Viewport", p_open)))
    {
        ImVec2 client_region_size_with_indent = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
        ImGui::GetWindowDrawList()->AddImage(g_backBufferTextureView.Get(), ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + client_region_size_with_indent);
    } 
    ImGui::End();
}

void Graphics::BeginFrame(const DirectXWindow* pWnd, const float clear_color[4])
{
    ResizeRenderTargetViews(pWnd);

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

    p_Context->ClearDepthStencilView(g_mainDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0U);
    p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), g_mainDepthStencilView.Get());
    ClearRTV(g_mainRenderTargetView.Get(), clear_color);
}


void Graphics::EndFrame()
{
    if (IsRenderingToImGui)
    {
        const float clear_color[4] = { 0.2f, 0.2f, 0.2f , 0.1f };
        MakeBackBufferTexture();
        ClearRTV(g_mainRenderTargetView.Get(), clear_color);
        static bool open = true;
        ShowRenderWindow(&open);
    }

    if (ImGuiEnabled)
    {
        ImGui::Render();
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
        {
            throw hrException(p_Device->GetDeviceRemovedReason());
        }
        else
        {
            throw hrException(hr);
        }
    }
}

void Graphics::DrawIndexed(UINT Count)
{
    p_Context->DrawIndexed(Count, 0U, 0U);
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

void Graphics::RecreateMainViews(const UINT& width, const UINT& height)
{
    g_mainDepthStencilView.Reset();
    g_mainRenderTargetView.Reset();
    p_SwapChain->ResizeBuffers(0U, width, height, DXGI_FORMAT_UNKNOWN, 0);
    CreateDepthStencilView();
    CreateBackBufferView();
}

void Graphics::RecreateGBufferViews(const UINT& width, const UINT& height)
{
    PositionTexture->Resize(*this, height, width);
    NormalTexture->Resize(*this, height, width);
    AlbedoTexture->Resize(*this, height, width);
    LightTexture->Resize(*this, height, width);

    CreateRTVForTexture(*PositionTexture, rtvPosition);
    CreateRTVForTexture(*NormalTexture, rtvNormal);
    CreateRTVForTexture(*AlbedoTexture, rtvAlbedo);
    CreateRTVForTexture(*LightTexture, rtvLight);
}

void Graphics::CreateDepthStencilView()
{
    ID3D11Texture2D* pBackBuffer;
    CHECK_HR(p_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));

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

    CHECK_HR ( p_Device->CreateTexture2D(&DSTextureDesc, nullptr, &pDepthStencil) ); 

    // Create view (same as we did with "View" on back buffer with render target view)
    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Texture2D.MipSlice = 0U;

    CHECK_HR ( p_Device->CreateDepthStencilView(pDepthStencil.Get(), &DSVDesc, &g_mainDepthStencilView) );
    pBackBuffer->Release();
}

void Graphics::CreateBackBufferView()
{
    ID3D11Texture2D* pBackBuffer;
    CHECK_HR(p_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
    CHECK_HR(p_Device->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView));
    pBackBuffer->Release();
}

void Graphics::CreateRTVForTexture(const Texture& texture, wrl::ComPtr<ID3D11RenderTargetView>& rtv)
{
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = texture.GetDesc().Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0U;
    CHECK_HR(p_Device->CreateRenderTargetView(texture.GetTexture(), &rtvDesc, &rtv));
}

void Graphics::MakeBackBufferTexture()
{
    ID3D11Texture2D* pBackBufferCopy = nullptr;
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_mainRenderTargetView->GetResource(reinterpret_cast<ID3D11Resource**>(&pBackBuffer));

    D3D11_TEXTURE2D_DESC descBack{};
    pBackBuffer->GetDesc(&descBack);

    D3D11_TEXTURE2D_DESC descBackCopy = descBack;
    descBackCopy.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    descBackCopy.MipLevels = 0U;
    descBackCopy.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    p_Device->CreateTexture2D(&descBackCopy, nullptr, &pBackBufferCopy);

    // because backbuffer haven't mipmap, we couldn't copy its texture just with 'CopyResource()', so we're update only most detailed mip-level
    p_Context->CopySubresourceRegion(pBackBufferCopy, 0U, 0U, 0U, 0U, pBackBuffer, 0U, nullptr);

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDescBuffer{};
    resourceViewDescBuffer.Format = descBack.Format;
    resourceViewDescBuffer.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDescBuffer.Texture2D.MostDetailedMip = 0U;
    resourceViewDescBuffer.Texture2D.MipLevels = ~0U;
    p_Device->CreateShaderResourceView(pBackBufferCopy, &resourceViewDescBuffer, &g_backBufferTextureView);

    // regenerate mipmap based on updated most detailed mip-level
    p_Context->GenerateMips(g_backBufferTextureView.Get());

    pBackBufferCopy->Release();
    pBackBuffer->Release();
}

void Graphics::ResizeRenderTargetViews(const DirectXWindow* pWnd)
{
    if (pWnd->GetResizeInfo().g_ResizeWidth != 0 && pWnd->GetResizeInfo().g_ResizeHeight != 0)
    {
        RecreateMainViews(pWnd->GetResizeInfo().g_ResizeWidth, pWnd->GetResizeInfo().g_ResizeHeight);
        RecreateGBufferViews(pWnd->GetResizeInfo().g_ResizeWidth, pWnd->GetResizeInfo().g_ResizeHeight);
        pWnd->ZeroResizeInfo();
        RECT rc;
        GetClientRect(pWnd->GetWnd(), &rc);
        D3D11_VIEWPORT viewport = D3D11_VIEWPORT{
            (FLOAT)rc.left,
            (FLOAT)rc.top,
            (FLOAT)(rc.right - rc.left),
            (FLOAT)(rc.bottom - rc.top),
            0.f,
            1.f
        };
        p_Context->RSSetViewports(1U, &viewport);
    }
}

void Graphics::SetProjection(dx::XMMATRIX projection) noexcept
{
    this->projection = projection;
}

dx::XMMATRIX Graphics::GetProjection() const noexcept
{
    return projection;
}

void Graphics::SetCamera(const Camera& new_cam)
{
    this->cam = new_cam;
}

void Graphics::SetAdditiveBlendingState()
{
    ID3D11BlendState* pBlendState = nullptr;

    D3D11_BLEND_DESC blendDesc{};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    CHECK_HR ( p_Device->CreateBlendState(&blendDesc, &pBlendState) );
    p_Context->OMSetBlendState(pBlendState, nullptr, 0xFFFFFFFF);

    pBlendState->Release();
}

void Graphics::ResetBlendingState()
{
    p_Context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

Camera Graphics::GetCamera() const
{
    return this->cam;
}

void Graphics::UnbindRenderTargets(UINT num_views)
{
    std::vector<ID3D11RenderTargetView*> null(num_views, nullptr);
    p_Context->OMSetRenderTargets(num_views, null.data(), nullptr);
}

void Graphics::UnbindPixelShaderResourses(UINT num_resourses)
{
    std::vector<ID3D11ShaderResourceView*> null(num_resourses, nullptr);
    p_Context->PSSetShaderResources(0U, num_resourses, null.data());
}

void Graphics::ClearRTV(ID3D11RenderTargetView* rtv, const float clear_color[4])
{
    assert(p_Context != nullptr);
    p_Context->ClearRenderTargetView(rtv, clear_color);
}
