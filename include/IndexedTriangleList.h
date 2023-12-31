#pragma once
#include <vector>
#include "DOK_assert.h"
#include "DOK_DX11.h"
#include "Vertex.h"
#include <DirectXMath.h>

struct FaceDesc
{
	FaceDesc(int i0, int i1, int i2)
	{
		indices[0] = std::move(i0);
		indices[1] = std::move(i1);
		indices[2] = std::move(i2);
	}
	int indices[3];
};

struct TanBitan
{
	dx::XMFLOAT3 tan;
	dx::XMFLOAT3 bitan;
};

static constexpr TanBitan GetTangentBitangent(
	const dx::XMFLOAT3 pos0, const dx::XMFLOAT3 pos1, const dx::XMFLOAT3 pos2,
	const dx::XMFLOAT2 UV0,  const dx::XMFLOAT2 UV1,  const dx::XMFLOAT2 UV2) noexcept
{
	using enum DynamicVertex::VertexLayout::ElementType;
	dx::XMFLOAT3 edge01 = pos0 - pos1;
	dx::XMFLOAT3 edge03 = pos0 - pos2;

	float deltaU01 = UV0.x - UV1.x;
	float deltaV01 = UV0.y - UV1.y;
	float deltaU03 = UV0.x - UV2.x;
	float deltaV03 = UV0.y - UV2.y;

	float f = 1.0f / (deltaU01 * deltaV03 - deltaU03 * deltaV01);

	dx::XMFLOAT3 tangent{};
	tangent.x = f * (deltaV03 * edge01.x - deltaV01 * edge03.x);
	tangent.y = f * (deltaV03 * edge01.y - deltaV01 * edge03.y);
	tangent.z = f * (deltaV03 * edge01.z - deltaV01 * edge03.z);

	dx::XMFLOAT3 bitangent{};
	bitangent.x = f * (-deltaU03 * edge01.x + deltaU01 * edge03.x);
	bitangent.y = f * (-deltaU03 * edge01.y + deltaU01 * edge03.y);
	bitangent.z = f * (-deltaU03 * edge01.z + deltaU01 * edge03.z);

	return { tangent, bitangent };
}

struct IndexedTriangleList
{	
	IndexedTriangleList() = default;
	IndexedTriangleList(DynamicVertex::VertexBuffer verts_in, std::vector<unsigned short> indxs)
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
		using Type = DynamicVertex::VertexLayout::ElementType;
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

	void SetNormalsSmooth() // Needs to be optimized
	{
		using enum DynamicVertex::VertexLayout::ElementType;
		assert ( vertices.GetLayout().HasSuchElement<Position3D>() && vertices.GetLayout().HasSuchElement<Normal>() );
		vertices.SetAllThe<Normal>(std::move(calc_mesh_normals(vertices.GetAllThe<Position3D>(), ConvertToFaces(indices))));
	}

	static dx::XMFLOAT3 calcFaceNormal(const std::vector<dx::XMFLOAT3>& verts, const FaceDesc& face)
	{
		using namespace DirectX;
		auto v0 = XMLoadFloat3(&verts[face.indices[0]]);
		auto v1 = XMLoadFloat3(&verts[face.indices[1]]);
		auto v2 = XMLoadFloat3(&verts[face.indices[2]]);

		const auto n = dx::XMVector3Normalize( dx::XMVector3Cross( (v1 - v0),(v2 - v0) ) );
		XMFLOAT3 normal;
		XMStoreFloat3(&normal, n);

		return normal;
	}
	static std::vector<dx::XMFLOAT3> calc_mesh_normals(const std::vector<dx::XMFLOAT3>& verts, const std::vector<FaceDesc>& faces)
	{
		std::vector<dx::XMFLOAT3> smoothNormals;
		smoothNormals.reserve(verts.size());

		std::vector<dx::XMFLOAT3> faceNormals;
		faceNormals.reserve(faces.size());

		for (const auto& face : faces)
		{
			faceNormals.push_back(calcFaceNormal(verts, face));
		}

		for (size_t vIndx = 0; vIndx < verts.size(); ++vIndx) // For each vertex
		{
			using namespace DirectX;
			std::vector<dx::XMFLOAT3> overlappingNormals;
			for (size_t faceIndx = 0; faceIndx < faces.size(); faceIndx++)
			{
				if (isOverlapped(vIndx, faces[faceIndx]))
					overlappingNormals.push_back(faceNormals[faceIndx]);
			}
			dx::XMVECTOR smoothNormal = dx::XMVectorSet(0.f,0.f,0.f,0.f);
			for (size_t i = 0; i < overlappingNormals.size(); i++)
			{
				smoothNormal += dx::XMVectorSet(overlappingNormals[i].x, overlappingNormals[i].y, overlappingNormals[i].z, 0.f);
			}
			dx::XMVector3Normalize(smoothNormal);
			smoothNormals.push_back({ XMVectorGetX(smoothNormal), XMVectorGetY(smoothNormal), XMVectorGetZ(smoothNormal) });
		}

		return smoothNormals;
	}
	static bool isAdjacent(const FaceDesc& face1, const FaceDesc& face2)
	{
		int similarIndxs = 0;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				if (face1.indices[i] == face2.indices[k])
					similarIndxs++;
			}
		}
		if (similarIndxs > 1)
			return true;
		return false;
	}
	static bool isOverlapped(const size_t& indx, const FaceDesc& face2)
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (face2.indices[i] == indx)
				return true;
		}
		return false;
	}
	static constexpr std::vector<FaceDesc> ConvertToFaces(const std::vector<unsigned short>& indices)
	{
		assert(indices.size() % 3 == 0);	
		std::vector<FaceDesc> result;
		result.reserve(indices.size() / 3);

		for (size_t i = 0; i < indices.size(); i+= 3)
		{
			result.push_back( FaceDesc(indices[i], indices[i + 1], indices[i + 2]) );
		}

		return result;
	}

	size_t GetTrinaglesCount() const noexcept
	{
		assert(indices.size() % 3 == 0);	
		return indices.size() / 3;
	}

	DynamicVertex::VertexBuffer vertices;
	std::vector<unsigned short> indices;
};