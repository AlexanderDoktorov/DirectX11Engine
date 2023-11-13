#pragma once
#include "IndexedTriangleList.h"
#include "DOK_assert.h"
#include "DOK_math.h"

class Polygon
{
public:
	
	static IndexedTriangleList Make(DynamicVertex::VertexLayout layout, const UINT& nVertex, const float& r = 1.f)
	{
		DOK_assert_noexit(nVertex > 2, L"Trying to make polygon with less than 3 vertices");
		dx::XMFLOAT3 normal = dx::XMFLOAT3(0.f, 0.f, -1.f); // 

		DynamicVertex::VertexBuffer vb(std::move(layout));

		for (unsigned int i = 0; i < nVertex; i++)
		{
			const float angle = i * 2 * PI / nVertex;
			vb.EmplaceBack(dx::XMFLOAT3({r* cosf(angle), r* sinf(angle), 0.f}), normal);
		}

		std::vector<unsigned short> indicies;
		for (unsigned int i = 1; i < nVertex - 1U; i++)
		{
			indicies.push_back(i);
			indicies.push_back(0);
			indicies.push_back(i + 1);
		}

		return { std::move(vb), std::move(indicies) };
	}
};