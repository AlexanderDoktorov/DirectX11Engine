#pragma once
#include <vector>
#include "DOK_assert.h"
#include <DirectXMath.h>

template<class Vertex>
struct IndexedTriangleList
{
	IndexedTriangleList(std::vector<Vertex> verts, std::vector<unsigned short> indxs)
		:
		vertices(std::move(verts)),
		indices(std::move(indxs))
	{
		DOK_assert(vertices.size() > (size_t)2, L"Wrong vertcies vector size");
		DOK_assert(indices.size() % 3 == 0, L"Wrong indicies count, must be devidable by 3");
	}

	void SetNormalsIndependentFlat()
	{
		using namespace DirectX;
		DOK_assert(indices.size() % 3 == 0 && indices.size() > 0, L"ERROR");

		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.pos);
			const auto p1 = XMLoadFloat3(&v1.pos);
			const auto p2 = XMLoadFloat3(&v2.pos);

			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			XMStoreFloat3(&v0.n, n);
			XMStoreFloat3(&v1.n, n);
			XMStoreFloat3(&v2.n, n);
		}
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};