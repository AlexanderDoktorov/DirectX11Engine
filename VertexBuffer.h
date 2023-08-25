#pragma once
#include <vector>
#include "IBindable.h"

class VertexBuffer : public IBindable
{
public:
    template <class T>
    VertexBuffer(Graphics& Gfx, const std::vector<T>& verts) : VertexStrides(sizeof(T))
    {
        D3D11_BUFFER_DESC vbd = {};
        vbd.ByteWidth = (UINT)(verts.size() * sizeof(T));
        vbd.Usage = D3D11_USAGE_DEFAULT;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = sizeof(T);
        D3D11_SUBRESOURCE_DATA vsd = {};
        vsd.pSysMem = verts.data();

        CHECK_HR ( GetDevice(Gfx)->CreateBuffer(&vbd, &vsd, &p_VertexBuffer) );
    }

    void Bind(Graphics& Gfx) noexcept override
    {
        UINT offset = 0U;
        GetContext(Gfx)->IASetVertexBuffers(0U, 1U, p_VertexBuffer.GetAddressOf(), &VertexStrides, &offset);
    }


protected:
    UINT VertexStrides;
    wrl::ComPtr<ID3D11Buffer> p_VertexBuffer;
};
