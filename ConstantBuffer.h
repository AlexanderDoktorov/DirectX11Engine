#pragma once
#include "IPipelineElement.h"

template <class T>
class ConstantBuffer : public IPipelineElement
{
public:
    ConstantBuffer(Graphics& Gfx, const T& CBData)
    {
        D3D11_BUFFER_DESC CBDesc = {};
        CBDesc.ByteWidth = sizeof(CBData);
        CBDesc.Usage = D3D11_USAGE_DYNAMIC;
        CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        D3D11_SUBRESOURCE_DATA CBSubData = {};
        CBSubData.pSysMem = &CBData;

        HRESULT hr = GetDevice(Gfx)->CreateBuffer(&CBDesc, &CBSubData, &p_ConstantBuffer); assert(SUCCEEDED(hr));
    }

    ConstantBuffer(Graphics& Gfx)
    {
        D3D11_BUFFER_DESC CBDesc = {};
        CBDesc.ByteWidth = sizeof(T);
        CBDesc.Usage = D3D11_USAGE_DYNAMIC;
        CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        HRESULT hr = GetDevice(Gfx)->CreateBuffer(&CBDesc, nullptr, &p_ConstantBuffer); assert(SUCCEEDED(hr));
    }

    void Update(Graphics& Gfx, const T& NewData)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        HRESULT hr = GetContext(Gfx)->Map(p_ConstantBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedSubresource); assert(SUCCEEDED(hr));

        memcpy(mappedSubresource.pData, &NewData, sizeof(NewData));

        GetContext(Gfx)->Unmap(p_ConstantBuffer.Get(), 0U);
    }

    ~ConstantBuffer() = default;

protected:
    wrl::ComPtr<ID3D11Buffer> p_ConstantBuffer;
};