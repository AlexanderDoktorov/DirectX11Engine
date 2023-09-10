#pragma once
#include <vector>
#include "DOK_assert.h"
#include <DirectXMath.h>

template<class Vertex>
struct IndexedTriangleStrip
{
	IndexedTriangleStrip(std::vector<Vertex> verts, std::vector<unsigned short> indxs)
		:
		vertices(std::move(verts)),
		indices(std::move(indxs))
	{
		DOK_assert(vertices.size() > (size_t)2, L"Wrong vertcies vector size");
		DOK_assert(indices.size() > (size_t)2, L"Wrong indices vector size");
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};