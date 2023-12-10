#include "HPipelineElements.h"
#include "ITexture2D.h"
#include "IBindable.h"
#include "Exceptions.h"
#include "Graphics.h"
#include "Material.h"
#include "XSResourse.h"
#include <assert.h>

#pragma comment(lib, "d3d11")

Graphics::Graphics(HWND hwnd) : 
    projection(dx::XMMATRIX()),
    ImGuiEnabled(true),
    IsRenderingToImGui(true)
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
    PositionTexture         = std::make_unique<RenderTexture>(*this, DXGI_FORMAT_R16G16B16A16_FLOAT, rc.bottom - rc.top, rc.right - rc.left);
    NormalTexture           = std::make_unique<RenderTexture>(*this, DXGI_FORMAT_R16G16B16A16_FLOAT, rc.bottom - rc.top, rc.right - rc.left);
    AlbedoTexture           = std::make_unique<RenderTexture>(*this, DXGI_FORMAT_R8G8B8A8_UNORM,     rc.bottom - rc.top, rc.right - rc.left);
    SpecularTexture         = std::make_unique<RenderTexture>(*this, DXGI_FORMAT_R8G8B8A8_UNORM,     rc.bottom - rc.top, rc.right - rc.left);
    LightTexture            = std::make_unique<RenderTexture>(*this, DXGI_FORMAT_R8G8B8A8_UNORM,     rc.bottom - rc.top, rc.right - rc.left);
    pLinearSampler          = std::make_unique<Sampler>(*this);
    
    // Create render targets for textures
    CreateRTVForTexture(PositionTexture.get(),   rtvPosition);
    CreateRTVForTexture(NormalTexture.get(),     rtvNormal);
    CreateRTVForTexture(AlbedoTexture.get(),     rtvAlbedo);
    CreateRTVForTexture(SpecularTexture.get(),   rtvSpecular);
    CreateRTVForTexture(LightTexture.get(),      rtvLight);

    pLightPassPixelShader   = std::make_unique<PixelShaderCommon>(*this,    L"shaders\\LightPS.cso");
    pScreenSpaceVS          = std::make_unique<VertexShaderCommon>(*this,   L"shaders\\ScreenSpaceVS.cso");
    pCombinePS              = std::make_unique<PixelShaderCommon>(*this,    L"shaders\\CombinePS.cso");

    // Initilize material system
    materialSystem.Initilize(*this, rc);

    pPixelCameraBuffer      = std::make_unique<PixelConstantBuffer<dx::XMFLOAT4>>(*this);

    ImGui_ImplDX11_Init(p_Device.Get(), p_Context.Get());
}

void Graphics::BeginGeometryPass(const DirectXWindow* pWnd)
{
    ResizeRenderTargetViews(pWnd);

    if (ImGuiEnabled)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    if (IsRenderingToImGui)
        ImGui::DockSpaceOverViewport();

    ID3D11RenderTargetView* rtvs[5] = { rtvPosition.Get(), rtvNormal.Get(), rtvAlbedo.Get(), rtvSpecular.Get(), materialSystem.rtvMaterialID.Get() };
    
    // Clear render targets
    const float rtvClear [4] = { 0.f,0.f,0.f,0.f };
    for (size_t i = 0; i < ARRAYSIZE(rtvs); i++)
    {
        p_Context->ClearRenderTargetView(rtvs[i], rtvClear);
    }
    const float rtvMaterialIdClear [4] = { -1.f,-1.f,-1.f,-1.f };
    p_Context->ClearRenderTargetView(materialSystem.rtvMaterialID.Get(), rtvMaterialIdClear);

    p_Context->ClearDepthStencilView(g_mainDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0U);
    p_Context->OMSetRenderTargets(ARRAYSIZE(rtvs), rtvs, g_mainDepthStencilView.Get());
}

void Graphics::EndGeometryPass()
{
    ID3D11RenderTargetView* nullRTVs[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
    p_Context->OMSetRenderTargets(ARRAYSIZE(nullRTVs), nullRTVs, nullptr);
}

void Graphics::BeginLightningPass()
{
    // Clear render target before fill the light info
    const float light_clear[4] = { 0.f,0.f,0.f,0.f };
    p_Context->ClearRenderTargetView(rtvLight.Get(), light_clear);
    // Set additive blending state to sum up results from all lights
    SetAdditiveBlendingState();

    // Bind shader resourses
    ID3D11ShaderResourceView* srvs[5] = { PositionTexture->GetSRV(), NormalTexture->GetSRV(), AlbedoTexture->GetSRV(), SpecularTexture->GetSRV(), materialSystem.MaterialIDTexture->GetSRV() };
    p_Context->OMSetRenderTargets(1U, rtvLight.GetAddressOf(), nullptr);
    p_Context->PSSetShaderResources(0U , ARRAYSIZE(srvs), srvs);

    // Set materials structured buffer
    materialSystem.pMaterialBuffer->Bind(*this);

    // Bind CameraBuffer
    const dx::XMFLOAT3 camPos = cam.GetPos();
    pPixelCameraBuffer->Update(*this, dx::XMFLOAT4(camPos.x, camPos.y, camPos.z, 0.f));
    pPixelCameraBuffer->Bind(*this);

    // Bind shaders
    pScreenSpaceVS->Bind(*this);
    pLightPassPixelShader->Bind(*this);

    // Bind linear sampler
    pLinearSampler->Bind(*this);
}

void Graphics::EndLightningPass()
{
    // Unbind resourses
    ID3D11ShaderResourceView* nullSRVs[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
    p_Context->PSSetShaderResources(0U, ARRAYSIZE(nullSRVs), nullSRVs);

    // Unbind render targets
    ID3D11RenderTargetView* nullRTV = nullptr;
    p_Context->OMSetRenderTargets(1U, &nullRTV, nullptr);

    // Unbind buffers
    pPixelCameraBuffer->Unbind(*this);

    // Unbind shaders
    pScreenSpaceVS->Unbind(*this);
    pLightPassPixelShader->Unbind(*this);

    ResetBlendingState();
}

void Graphics::PerformCombinePass()
{
    const float light_clear[4] = { 0.1f,0.1f,0.1f,0.3f };
    
    p_Context->IASetInputLayout(nullptr);
    p_Context->ClearRenderTargetView(g_mainRenderTargetView.Get(), light_clear);

    ID3D11ShaderResourceView* srvs[2] = { AlbedoTexture->GetSRV(), LightTexture->GetSRV() };

    p_Context->PSSetShaderResources(0U, ARRAYSIZE(srvs), srvs);
    p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), nullptr);
    pCombinePS->Bind(*this);
    pScreenSpaceVS->Bind(*this);
    Draw(3U);

    // Unbind shader resourses
    ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
    p_Context->PSSetShaderResources(0U, 2U, nullSRV);

    // Unbind render targets
    ID3D11RenderTargetView* nullRTV = nullptr;
    p_Context->OMSetRenderTargets(1U, &nullRTV, nullptr);

    // Unbind shaders
    pCombinePS->Unbind(*this);
    pScreenSpaceVS->Unbind(*this);
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
    p_Context->ClearRenderTargetView(g_mainRenderTargetView.Get(), clear_color);
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

Graphics::MaterialSystem& Graphics::GetMaterialSystem() noexcept
{
    return materialSystem;
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
    SpecularTexture->Resize(*this, height, width);

    CreateRTVForTexture(PositionTexture.get(), rtvPosition);
    CreateRTVForTexture(NormalTexture.get(), rtvNormal);
    CreateRTVForTexture(AlbedoTexture.get(), rtvAlbedo);
    CreateRTVForTexture(LightTexture.get(), rtvLight);
    CreateRTVForTexture(SpecularTexture.get(), rtvSpecular);
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

void Graphics::CreateRTVForTexture(const ITexture2D* texture, wrl::ComPtr<ID3D11RenderTargetView>& rtv)
{
    assert(texture->GetDesc().BindFlags | D3D11_BIND_RENDER_TARGET);

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = texture->GetDesc().Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0U;
    CHECK_HR(p_Device->CreateRenderTargetView(texture->GetTexture(), &rtvDesc, &rtv));
}

wrl::ComPtr<ID3D11ShaderResourceView> Graphics::MakeSRVFromRTV(wrl::ComPtr<ID3D11RenderTargetView> rtv)
{
    wrl::ComPtr<ID3D11ShaderResourceView> result;
    ID3D11Texture2D* pRenderTextureCopy = nullptr;
    ID3D11Texture2D* pRenderTexture = nullptr;
    rtv->GetResource(reinterpret_cast<ID3D11Resource**>(&pRenderTexture));

    D3D11_TEXTURE2D_DESC descText{};
    pRenderTexture->GetDesc(&descText);

    D3D11_TEXTURE2D_DESC descTextCopy = descText;
    descTextCopy.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    descTextCopy.MipLevels = 0U;
    descTextCopy.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    CHECK_HR( p_Device->CreateTexture2D(&descTextCopy, nullptr, &pRenderTextureCopy) );

    // because backbuffer haven't mipmap, we couldn't copy its texture just with 'CopyResource()', so we're update only most detailed mip-level
    p_Context->CopySubresourceRegion(pRenderTextureCopy, 0U, 0U, 0U, 0U, pRenderTexture, 0U, nullptr);

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDescBuffer{};
    resourceViewDescBuffer.Format = descText.Format;
    resourceViewDescBuffer.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDescBuffer.Texture2D.MostDetailedMip = 0U;
    resourceViewDescBuffer.Texture2D.MipLevels = ~0U;
    CHECK_HR( p_Device->CreateShaderResourceView(pRenderTextureCopy, &resourceViewDescBuffer, &result) );

    // regenerate mipmap based on updated most detailed mip-level
    p_Context->GenerateMips(result.Get());

    pRenderTextureCopy->Release();
    pRenderTexture->Release();

    return result;
}

wrl::ComPtr<ID3D11RenderTargetView> Graphics::MakeRTVFromTexture(ID3D11Device* p_Device, const ITexture2D* texture)
{
    assert(texture->GetDesc().BindFlags | D3D11_BIND_RENDER_TARGET);

    wrl::ComPtr<ID3D11RenderTargetView> rtv;

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = texture->GetDesc().Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0U;
    CHECK_HR(p_Device->CreateRenderTargetView(texture->GetTexture(), &rtvDesc, &rtv));

    return rtv;
}

void Graphics::ResizeRenderTargetViews(const DirectXWindow* pWnd)
{
    UINT w = pWnd->GetResizeInfo().g_ResizeWidth;
    UINT h = pWnd->GetResizeInfo().g_ResizeHeight;
    if (w != 0 && h != 0)
    {
        RecreateMainViews(w, h);
        RecreateGBufferViews(w, h);
        materialSystem.OnResize(w, h);
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

void Graphics::ResetBlendingState()
{
    p_Context->OMSetBlendState(nullptr, nullptr, UINT_MAX);
}

Camera Graphics::GetCamera() const
{
    return this->cam;
}

// (********************) Material System (********************) //
//                                                               //
void Graphics::MaterialSystem::Initilize(Graphics& Gfx, const RECT& rc) noexcept
{
    pGfx = &Gfx;
    MaterialIDTexture   = std::make_unique<RenderTexture>(*pGfx, DXGI_FORMAT_R32_SINT, rc.bottom - rc.top, rc.right - rc.left);
    pMaterialBuffer     = std::make_unique<MaterialBuffer>(*pGfx, SLOT_MATERIAL_STRUCTURED_BUFFER);
    rtvMaterialID       = Graphics::MakeRTVFromTexture(pGfx->p_Device.Get(), MaterialIDTexture.get());
}

void Graphics::MaterialSystem::OnResize(UINT resizeWidth, UINT resizeHeight) noexcept
{
    MaterialIDTexture->Resize(*pGfx, resizeHeight, resizeWidth);
    rtvMaterialID = Graphics::MakeRTVFromTexture(pGfx->p_Device.Get(), MaterialIDTexture.get());
}

size_t Graphics::MaterialSystem::GetMaterialIndex(Material& material) noxnd
{
    if (std::optional<size_t> indx = IsLoaded(material))
        return indx.value();
    else
    {
        auto newIndex = loadedMaterials.size();
        assert(loadedMaterials.size() <= MAX_MATERIALS);
        pMaterialBuffer->Update(*pGfx, material.GetMaterialDesc(), newIndex);
        loadedMaterials.push_back(std::make_unique<Material>(std::move(material)));
        return newIndex;
    }
}

bool Graphics::MaterialSystem::UpdateMaterialAt(size_t indx) noexcept
{
    if (indx >= MAX_MATERIALS || !pMaterialBuffer.get())
        return false;
    if (auto pMat = GetMaterialAt(indx))
    {
        pMaterialBuffer->Update(*pGfx, pMat->GetMaterialDesc(), indx);
        return true;
    }
    return false;
}

Material* Graphics::MaterialSystem::GetMaterialAt(size_t indx) noexcept
{
    if (indx < loadedMaterials.size())
        return loadedMaterials.at(indx).get();
    return nullptr;
}

std::optional<size_t> Graphics::MaterialSystem::IsLoaded(const Material& material) const noexcept
{
    for (size_t i = 0U; i < loadedMaterials.size(); i++)
    {
        if (loadedMaterials[i]->GetDirectory() == material.GetDirectory() && loadedMaterials[i]->GetName() == material.GetName())
            return i;
    }
    return std::nullopt;

}
void Graphics::MaterialSystem::ShowMaterialsWindow(bool* p_open) noexcept
{
    if (ImGui::Begin("Loaded materials", p_open))
    {
        for (size_t i = 0; i < loadedMaterials.size(); i++)
        {
            if (loadedMaterials[i]->ShowMaterialGUI())
                UpdateMaterialAt(i);
        }
    }
    ImGui::End();
}
//                                                               //
//***************************************************************//
