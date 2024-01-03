#include "IndexBuffer.h"
#include <assert.h>

IndexBuffer::IndexBuffer(Graphics& Gfx, const std::string& nameTag, const std::vector<unsigned short>& indicies)
	:
	Count((UINT)indicies.size()),
	nameTag(nameTag)
{
	D3D11_BUFFER_DESC IBD = {};
	IBD.ByteWidth = (UINT)(sizeof(unsigned short) * Count);
	IBD.Usage = D3D11_USAGE_IMMUTABLE;
	IBD.StructureByteStride = sizeof(unsigned short);
	IBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ISD = {};
	ISD.pSysMem = indicies.data();

	CHECK_HR ( GetDevice(Gfx)->CreateBuffer(&IBD, &ISD, &p_IndexBuffer) );
}

IndexBuffer::IndexBuffer(Graphics& Gfx, const std::vector<unsigned short>& indicies) : IndexBuffer(Gfx, "TAG_?", indicies)
{

}

void IndexBuffer::Bind(Graphics& Gfx) noexcept
{
	GetContext(Gfx)->IASetIndexBuffer(p_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0U);
}

std::string IndexBuffer::GetID() const noexcept
{
	return std::string(typeid(IndexBuffer).name()).append("#").append(nameTag);
}

std::string IndexBuffer::GenerateID(const std::string& nameTag, const std::vector<unsigned short>&)
{
	return std::string(typeid(IndexBuffer).name()).append("#").append(nameTag);
}

std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(Graphics& Gfx, const std::string& nameTag, const std::vector<unsigned short>& indices) noxnd
{
	assert(nameTag != "TAG_?");
	return BindableSystem::Resolve<IndexBuffer>(Gfx, nameTag, indices);
}

UINT IndexBuffer::GetCount() const noexcept
{ 
	return Count; 
}

