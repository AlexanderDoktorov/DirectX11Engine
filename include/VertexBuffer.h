#pragma once
#include "IBindable.h"
#include "Vertex.h"
#include "hrException.h"
#include <wrl.h>

class VertexBuffer : public IBindable
{
public:
	VertexBuffer( Graphics& gfx,const DynamicVertex::VertexBuffer& vbuf );
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& verts) : stride( (UINT)sizeof(V) )
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT( verts.size() * sizeof(V) );
		bd.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = verts.data();

		HRESULT hr = GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer); CHECK_HR(hr);
	}

	void Bind( Graphics& gfx ) noexcept override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
