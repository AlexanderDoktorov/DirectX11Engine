#pragma once

#include "IndexedTriangleList.h"
#include "DOK_DX11.h"

class FlatRect
{
public:
	using enum DynamicVertex::VertexLayout::ElementType;
	typedef dx::XMFLOAT2 UV;

	static IndexedTriangleList Make(DynamicVertex::VertexLayout layout)
	{
		DynamicVertex::VertexBuffer vb{ std::move( layout ), 4U };

		vb[0].Attr<Position3D>() = dx::XMFLOAT3(1.f, -1.f, 0.f);
		vb[1].Attr<Position3D>() = dx::XMFLOAT3(1.f, 1.f, 0.f);
		vb[2].Attr<Position3D>() = dx::XMFLOAT3(-1.f, -1.f, 0.f);
		vb[3].Attr<Position3D>() = dx::XMFLOAT3(-1.f, 1.f, 0.f);

		return 
		{ 
			std::move(vb),
			{
				0,2,1,
				2,3,1
			}
		};
	}

	static IndexedTriangleList MakeNormalized(DynamicVertex::VertexLayout layout)
	{
		auto itl = Make(layout);

		itl.vertices[0].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);
		itl.vertices[1].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);
		itl.vertices[2].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);
		itl.vertices[3].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);

		return itl;
	}

	static IndexedTriangleList MakeNormalizedTextured(DynamicVertex::VertexLayout layout)
	{
		auto itl = MakeNormalized(layout);

		itl.vertices[0].Attr<Texture2D>() = UV(0.f,0.f);
		itl.vertices[1].Attr<Texture2D>() = UV(1.f,0.f);
		itl.vertices[2].Attr<Texture2D>() = UV(0.f,1.f);
		itl.vertices[3].Attr<Texture2D>() = UV(1.f,1.f);

		return itl;
	}

	static IndexedTriangleList MakeNormalizedTexturedTBN(DynamicVertex::VertexLayout layout)
	{
		auto itl = MakeNormalizedTextured(layout);

		dx::XMFLOAT3 edge01 = itl.vertices[0].Attr<Position3D>() - itl.vertices[1].Attr<Position3D>();
		dx::XMFLOAT3 edge03 = itl.vertices[0].Attr<Position3D>() - itl.vertices[3].Attr<Position3D>();

		float deltaU01 = itl.vertices[0].Attr<Texture2D>().x - itl.vertices[1].Attr<Texture2D>().x;
		float deltaV01 = itl.vertices[0].Attr<Texture2D>().y - itl.vertices[1].Attr<Texture2D>().y;
		float deltaU03 = itl.vertices[0].Attr<Texture2D>().x - itl.vertices[3].Attr<Texture2D>().x;
		float deltaV03 = itl.vertices[0].Attr<Texture2D>().y - itl.vertices[3].Attr<Texture2D>().y;

		float f = 1.0f / (deltaU01 * deltaV03 - deltaU03 * deltaV01);

		dx::XMFLOAT3 tangent{};
		tangent.x = f * (deltaV03 * edge01.x - deltaV01 * edge03.x);
		tangent.y = f * (deltaV03 * edge01.y - deltaV01 * edge03.y);
		tangent.z = f * (deltaV03 * edge01.z - deltaV01 * edge03.z);

		dx::XMFLOAT3 bitangent{};
		bitangent.x = f * (-deltaU03 * edge01.x + deltaU01 * edge03.x);
		bitangent.y = f * (-deltaU03 * edge01.y + deltaU01 * edge03.y);
		bitangent.z = f * (-deltaU03 * edge01.z + deltaU01 * edge03.z);

		itl.vertices[0].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);
		itl.vertices[1].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);
		itl.vertices[2].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);
		itl.vertices[3].Attr<Normal>() = dx::XMFLOAT3(0.f, 0.f, 1.f);

		itl.vertices[0].Attr<Tangent>() = tangent;
		itl.vertices[1].Attr<Tangent>() = tangent;
		itl.vertices[2].Attr<Tangent>() = tangent;
		itl.vertices[3].Attr<Tangent>() = tangent;

		itl.vertices[0].Attr<Bitangent>() = bitangent;
		itl.vertices[1].Attr<Bitangent>() = bitangent;
		itl.vertices[2].Attr<Bitangent>() = bitangent;
		itl.vertices[3].Attr<Bitangent>() = bitangent;

		return itl;
	}

};