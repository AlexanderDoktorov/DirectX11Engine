#pragma once
#include <vector>
#include "DOK_assert.h"
#include "Vertex.h"
#include <DirectXMath.h>

namespace Vertex = DynamicVertex;

struct IndexedTriangleList
{
	IndexedTriangleList() = default;
	IndexedTriangleList(Vertex::VertexBuffer verts_in, std::vector<unsigned short> indxs)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indxs))
	{
		DOK_assert(vertices.Size() > 2, L"Wrong vertcies vector size");
		DOK_assert(indices.size() % 3 == 0, L"Wrong indicies count, must be devidable by 3");
	}

	void SetNormalsIndependentFlat()
	{
		using namespace DirectX;
		using Type = Vertex::VertexLayout::ElementType;
		for( size_t i = 0; i < indices.size(); i += 3 )
		{
			auto v0 = vertices[indices[i]];
			auto v1 = vertices[indices[i + 1]];
			auto v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3( &v0.Attr<Type::Position3D>() );
			const auto p1 = XMLoadFloat3( &v1.Attr<Type::Position3D>() );
			const auto p2 = XMLoadFloat3( &v2.Attr<Type::Position3D>() );

			const auto n = XMVector3Normalize( XMVector3Cross( (p1 - p0),(p2 - p0) ) );

			XMStoreFloat3( &v0.Attr<Type::Normal>(),n );
			XMStoreFloat3( &v1.Attr<Type::Normal>(),n );
			XMStoreFloat3( &v2.Attr<Type::Normal>(),n );
		}
	}

	Vertex::VertexBuffer vertices;
	std::vector<unsigned short> indices;
};