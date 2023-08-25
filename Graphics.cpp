#include "Graphics.h"
#include "Exceptions.h"
#include <assert.h>

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
    ReCreateRenderTargetView();
    ReCreateDepthStencilView();
    p_Context->OMSetRenderTargets(1U, g_mainRenderTargetView.GetAddressOf(), g_mainDepthStencilView.Get());

    RECT rc;
    GetClientRect(hwnd, &rc);
    vp = D3D11_VIEWPORT{
        0.f,
        0.f,
        (FLOAT)(rc.right - rc.left),
        (FLOAT)(rc.bottom - rc.top),
        0.f,
        1.f
    };
    p_Context->RSSetViewports(1U, &vp);

    // RSState
    D3D11_RASTERIZER_DESC RSDesc{};
    RSDesc.CullMode = D3D11_CULL_BACK;
    RSDesc.FillMode = D3D11_FILL_SOLID;

    p_Device->CreateRasterizerState(&RSDesc, &p_RSState);

    ImGui_ImplDX11_Init(p_Device.Get(), p_Context.Get());
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

Camera Graphics::GetCamera() const
{
    return this->cam;
}

void Graphics::ShowRenderWindow(bool* p_open)
{
    //ReCreateShadersResourseView();
    if (ImGui::Begin("Viewport", p_open))
    {
        ImVec2 client_region_size_with_indent = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
        ImGui::GetWindowDrawList()->AddImage(g_mainShaderResourseView.Get(), ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + client_region_size_with_indent);
    } 
    ImGui::End();
}

void Graphics::EndFrame()
{
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

Graphics::~Graphics()
{
    ImGui_ImplDX11_Shutdown();
}

void Graphics::ResizeBackBuffer(const UINT& width, const UINT& height)
{
    p_SwapChain->ResizeBuffers(0U, width, height, DXGI_FORMAT_UNKNOWN, 0);
    ReCreateDepthStencilView();
    ReCreateRenderTargetView();
}

void Graphics::ReCreateDepthStencilView()
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

void Graphics::ReCreateRenderTargetView()
{
    ID3D11Texture2D* pBackBuffer;
    CHECK_HR(p_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
    CHECK_HR(p_Device->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView));
    pBackBuffer->Release();
}

void Graphics::SetShaderResourses()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_mainRenderTargetView->GetResource(reinterpret_cast<ID3D11Resource**>(&pBackBuffer));

    D3D11_TEXTURE2D_DESC descBack = { };
    pBackBuffer->GetDesc(&descBack);

    D3D11_TEXTURE2D_DESC descBackCopy = descBack;
    descBackCopy.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    descBackCopy.MipLevels = 0U;
    descBackCopy.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    p_Device->CreateTexture2D(&descBackCopy, nullptr, &pBackBufferCopy);

    // because backbuffer haven't mipmap, we couldn't copy its texture just with 'CopyResource()', so we're update only most detailed mip-level
    p_Context->CopySubresourceRegion(pBackBufferCopy.Get(), 0U, 0U, 0U, 0U, pBackBuffer, 0U, nullptr);

    pBackBuffer->Release();

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDescBuffer = { };
    resourceViewDescBuffer.Format = descBack.Format;
    resourceViewDescBuffer.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDescBuffer.Texture2D.MostDetailedMip = 0U;
    resourceViewDescBuffer.Texture2D.MipLevels = ~0U;
    p_Device->CreateShaderResourceView(pBackBufferCopy.Get(), &resourceViewDescBuffer, &g_mainShaderResourseView);

    // regenerate mipmap based on updated most detailed mip-level
    p_Context->GenerateMips(g_mainShaderResourseView.Get());
}

//void func()
//{
//    static ID3D11Texture2D* pBackBufferCopy = nullptr;
//    static ID3D11ShaderResourceView* pBackBufferResource = nullptr;
//
//    ID3D11Texture2D* pBackBuffer = nullptr;
//    g_mainRenderTargetView->GetResource(reinterpret_cast<ID3D11Resource**>(&pBackBuffer));
//
//    D3D11_TEXTURE2D_DESC descBack = { };
//    pBackBuffer->GetDesc(&descBack);
//
//    if (pBackBufferCopy == nullptr)
//    {
//        D3D11_TEXTURE2D_DESC descBackCopy = descBack;
//        descBackCopy.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//        descBackCopy.MipLevels = 0U;
//        descBackCopy.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
//        p_Device->CreateTexture2D(&descBackCopy, nullptr, &pBackBufferCopy);
//    }
//
//    // because backbuffer haven't mipmap, we couldn't copy its texture just with 'CopyResource()', so we're update only most detailed mip-level
//    p_Context->CopySubresourceRegion(pBackBufferCopy, 0U, 0U, 0U, 0U, pBackBuffer, 0U, nullptr);
//
//    pBackBuffer->Release();
//
//    if (pBackBufferResource == nullptr)
//    {
//        D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDescBuffer = { };
//        resourceViewDescBuffer.Format = descBack.Format;
//        resourceViewDescBuffer.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//        resourceViewDescBuffer.Texture2D.MostDetailedMip = 0U;
//        resourceViewDescBuffer.Texture2D.MipLevels = ~0U;
//        p_Device->CreateShaderResourceView(pBackBufferCopy, &resourceViewDescBuffer, &pBackBufferResource);
//    }
//
//    // regenerate mipmap based on updated most detailed mip-level
//    p_Context->GenerateMips(pBackBufferResource);
//
//    ImVec2 vecMin = { 10.f, 10.f };
//    ImVec2 vecMax = { vecMin.x + floorf((float)descBack.Width / 4.0f), vecMin.y + floorf((float)descBack.Height / 4.0f) };
//
//    if (ImGui::Begin("Viewport", p_open))
//    {
//        ImVec2 client_region_size_with_indent = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
//        ImGui::GetWindowDrawList()->AddImage(pBackBufferResource, ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + client_region_size_with_indent);
//    }
//    ImGui::End();
//}

