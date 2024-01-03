#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics& gfx, std::string buff_tag, const DynamicVertex::VertexBuffer& vbuf)
	:
	stride( (UINT)vbuf.GetLayout().Size() ),
	v_Tag(buff_tag)
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT( vbuf.SizeBytes() );
	bd.StructureByteStride = stride;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vbuf.GetData();
	CHECK_EXPR_DEFINE_HR( GetDevice( gfx )->CreateBuffer( &bd,&sd,&pVertexBuffer ) );
}

std::string VertexBuffer::GenerateID(std::string buff_tag, const DynamicVertex::VertexBuffer& vbuf) noexcept
{
	return std::string(typeid(VertexBuffer).name()).append(buff_tag).append("#").append(vbuf.GetLayout().GetCode());
}

std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(Graphics& gfx, std::string buff_tag, const DynamicVertex::VertexBuffer& vbuf) noexcept
{
	return BindableSystem::Resolve<VertexBuffer>(gfx, buff_tag, vbuf);
}

void VertexBuffer::Bind( Graphics& gfx ) noexcept
{
	const UINT offset = 0u;
	GetContext( gfx )->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
}