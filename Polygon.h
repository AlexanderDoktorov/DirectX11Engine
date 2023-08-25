#pragma once
#include "IndexedTriangleList.h"
#include "DOK_assert.h"
#include "DOK_math.h"

class Polygon
{
public:
	template<class V>
	static IndexedTriangleList<V> Make(const UINT& nVertex, const float& r = 1.f)
	{
		DOK_assert_noexit(nVertex > 2, L"Trying to make polygon with less than 3 vertices");

		std::vector<V> vertices(nVertex);
		for (unsigned int i = 0; i < nVertex; i++)
		{
			const float angle = i * 2 * PI / nVertex;
			vertices[i].pos = { r * cosf(angle), r * sinf(angle), 0.f};
		}

		std::vector<unsigned short> indicies;
		for (unsigned int i = 1; i < nVertex - 1U; i++)
		{
			indicies.push_back(i);
			indicies.push_back(0);
			indicies.push_back(i + 1);
		}

		return { std::move(vertices), std::move(indicies) };
	}
};