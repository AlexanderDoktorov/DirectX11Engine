#pragma once
#include "IBindable.h"
#include "Vertex.h"
#include "Exceptions.h"

class VertexBuffer : public IBindable
{
public:
	VertexBuffer( Graphics& gfx,const Vertex::VertexBuffer& vbuf );

	void Bind( Graphics& gfx ) noexcept override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
