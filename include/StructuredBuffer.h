#pragma once
#include "IBindable.h"
#include "ISlot.h"
#include "IShaderResourse.h"
#include <array>

template <class T, size_t numStructs>
class StructuredBuffer : public IBindable, public ISlot, public IShaderResourse
{
public:
    StructuredBuffer(Graphics& Gfx, const std::array<T, numStructs>& SBData)
    {
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.ByteWidth = sizeof(T) * numStructs; // numElements is the number of elements in the buffer
        bufferDesc.StructureByteStride = sizeof(T);
        bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        D3D11_SUBRESOURCE_DATA CBSubData = {};
        CBSubData.pSysMem = &SBData.data();

        HRESULT hr = GetDevice(Gfx)->CreateBuffer(&bufferDesc, &CBSubData, &pStructuredBuffer); assert(SUCCEEDED(hr));

        // Create the shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffers
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        srvDesc.BufferEx.FirstElement = 0;
        srvDesc.BufferEx.NumElements = numStructs;
         
        hr = GetDevice(Gfx)->CreateShaderResourceView(pStructuredBuffer.Get(), &srvDesc, &pShaderResourseView); assert(SUCCEEDED(hr));
    }

    StructuredBuffer(Graphics& Gfx, const T* SBData)
    {
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.ByteWidth = sizeof(T) * numStructs; // numElements is the number of elements in the buffer
        bufferDesc.StructureByteStride = sizeof(T);
        bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        D3D11_SUBRESOURCE_DATA CBSubData = {};
        CBSubData.pSysMem = SBData;

        HRESULT hr = GetDevice(Gfx)->CreateBuffer(&bufferDesc, &CBSubData, &pStructuredBuffer); assert(SUCCEEDED(hr));

        // Create the shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffers
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        srvDesc.BufferEx.FirstElement = 0;
        srvDesc.BufferEx.NumElements = numStructs;

        hr = GetDevice(Gfx)->CreateShaderResourceView(pStructuredBuffer.Get(), &srvDesc, &pShaderResourseView); assert(SUCCEEDED(hr));
    }

    StructuredBuffer(Graphics& Gfx)
    {
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.ByteWidth = sizeof(T) * numStructs; // numElements is the number of elements in the buffer
        bufferDesc.StructureByteStride = sizeof(T);
        bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        HRESULT hr = GetDevice(Gfx)->CreateBuffer(&bufferDesc, nullptr, &pStructuredBuffer); assert(SUCCEEDED(hr));

        // Create the shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Use DXGI_FORMAT_UNKNOWN for structured buffers
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        srvDesc.BufferEx.FirstElement = 0;
        srvDesc.BufferEx.NumElements = numStructs;

        HRESULT hr = GetDevice(Gfx)->CreateShaderResourceView(pStructuredBuffer.Get(), &srvDesc, &pShaderResourseView); assert(SUCCEEDED(hr));
    }

    void Update(Graphics& Gfx, const std::array<T, numStructs>& newSBData)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        HRESULT hr = GetContext(Gfx)->Map(pStructuredBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedSubresource); assert(SUCCEEDED(hr));

        memcpy(mappedSubresource.pData, newSBData.data(), sizeof(T) * numStructs);

        GetContext(Gfx)->Unmap(pStructuredBuffer.Get(), 0U);
    }

    void SetBindSlot(UINT slot) noexcept override
    {
        bindSlot = slot;
    }
    UINT GetBindSlot() const noexcept override
    {
        return bindSlot;
    }
    ID3D11ShaderResourceView* GetSRV() const noexcept override
    {
        return pShaderResourseView.Get();
    }

    ~StructuredBuffer() = default;

protected:
    UINT bindSlot = 0U;
    wrl::ComPtr<ID3D11Buffer> pStructuredBuffer;
    wrl::ComPtr<ID3D11ShaderResourceView> pShaderResourseView;
};