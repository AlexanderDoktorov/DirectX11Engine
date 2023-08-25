#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "IBindable.h"

class IndexBuffer : public IBindable
{
public:
	IndexBuffer(Graphics& Gfx, const std::vector<unsigned short>& indicies)
		:
		Count((UINT)indicies.size())
	{
		D3D11_BUFFER_DESC IBD = {};
		IBD.ByteWidth = (UINT)(sizeof(unsigned short) * Count);
		IBD.Usage = D3D11_USAGE_DEFAULT;
		IBD.StructureByteStride = sizeof(unsigned short);
		IBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA ISD = {};
		ISD.pSysMem = indicies.data();

		CHECK_HR ( GetDevice(Gfx)->CreateBuffer(&IBD, &ISD, &p_IndexBuffer) );
	}

	void Bind(Graphics& Gfx) noexcept
	{
		GetContext(Gfx)->IASetIndexBuffer(p_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0U);
	}

	UINT GetCount() const { return Count; }

protected:
	UINT Count;
	wrl::ComPtr<ID3D11Buffer> p_IndexBuffer;
};
