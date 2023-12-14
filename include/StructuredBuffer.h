#pragma once
#include "IBindable.h"
#include "ISlot.h"
#include "XSResourse.h"
#include <wrl.h>
#include <array>

namespace wrl = Microsoft::WRL;

template <class T, size_t numStructs, class ...ShaderTypes>
class StructuredBuffer : public ComboSResrouse<ShaderTypes...>
{
    using ISlot::SetBindSlot;
    using GraphicsChild::GetDevice;
    using GraphicsChild::GetContext;
public:
    StructuredBuffer(Graphics& Gfx, const std::array<T, numStructs>& SBData) : Slotted()
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

    StructuredBuffer(Graphics& Gfx, UINT bindSlot) : StructuredBuffer(Gfx)
    {
        SetBindSlot(bindSlot);
    }

    StructuredBuffer(Graphics& Gfx)
    {
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

        hr = GetDevice(Gfx)->CreateShaderResourceView(pStructuredBuffer.Get(), &srvDesc, &pShaderResourseView); assert(SUCCEEDED(hr));
    }

    void Update(Graphics& Gfx, const std::array<T, numStructs>& newSBData)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        HRESULT hr = GetContext(Gfx)->Map(pStructuredBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedSubresource); assert(SUCCEEDED(hr));

        memcpy(mappedSubresource.pData, newSBData.data(), sizeof(T) * numStructs);

        GetContext(Gfx)->Unmap(pStructuredBuffer.Get(), 0U);
    }
    void Update(Graphics& Gfx, const T& data, size_t indx)
    {
        assert(indx < numStructs);
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        HRESULT hr = GetContext(Gfx)->Map(pStructuredBuffer.Get(), 0U, D3D11_MAP_WRITE_NO_OVERWRITE , 0U, &mappedSubresource); assert(SUCCEEDED(hr));

        // Calculate the offset to the particular struct
        size_t offset = indx * sizeof(T);

        // Copy the new data to the mapped resource
        ::memcpy(static_cast<char*>(mappedSubresource.pData) + offset, &data, sizeof(T));

        GetContext(Gfx)->Unmap(pStructuredBuffer.Get(), 0U);
    }
    T GetData(Graphics& Gfx, size_t indx)
    {
        assert(indx < numStructs);
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        HRESULT hr = GetContext(Gfx)->Map(pStructuredBuffer.Get(), 0U, D3D11_MAP_WRITE_NO_OVERWRITE , 0U, &mappedSubresource); assert(SUCCEEDED(hr));

        // Calculate the offset to the particular struct
        size_t offset = indx * sizeof(T);

        // Copy the new data to the mapped resource
        T data = *reinterpret_cast<T*>(static_cast<char*>(mappedSubresource.pData) + offset);

        GetContext(Gfx)->Unmap(pStructuredBuffer.Get(), 0U);
        return data;
    }
    ID3D11ShaderResourceView* GetSRV() const noexcept override
    {
        return pShaderResourseView.Get();
    }

    ~StructuredBuffer() = default;

protected:
    wrl::ComPtr<ID3D11Buffer> pStructuredBuffer;
    wrl::ComPtr<ID3D11ShaderResourceView> pShaderResourseView;
};