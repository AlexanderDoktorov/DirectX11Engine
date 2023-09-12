#pragma once
#include "IndexedTriangleList.h"
#include "IndexedTriangleStrip.h"
#include "DOK_math.h"
class Sphere
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int latDiv, int longDiv)
	{
		namespace dx = DirectX;
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		constexpr float radius = 1.0f;
		const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationX(lattitudeAngle * iLat)
			);
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					latBase,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		// add the cap vertices
		const auto iNorthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3(&vertices.back().pos, base);
		const auto iSouthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

		const auto calcIdx = [latDiv, longDiv](unsigned short iLat, unsigned short iLong)
		{ return iLat * longDiv + iLong; };
		std::vector<unsigned short> indices;
		for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return { std::move(vertices),std::move(indices) };
	}

	template <class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentTexturedNormalized(int numSlices, int numStacks, float radius)
	{
		std::vector<V> vertices;
		std::vector<unsigned short> indices;

		for (int i = 0; i <= numStacks; ++i)
		{
			float phi = DirectX::XM_PI * static_cast<float>(i) / static_cast<float>(numStacks);
			for (int j = 0; j <= numSlices; ++j)
			{
				float theta = DirectX::XM_2PI * static_cast<float>(j) / static_cast<float>(numSlices);

				V vertex;
				vertex.pos.x = radius * sinf(phi) * cosf(theta);
				vertex.pos.y = radius * cosf(phi);
				vertex.pos.z = radius * sinf(phi) * sinf(theta);
				vertex.tc.x = static_cast<float>(j) / static_cast<float>(numSlices);
				vertex.tc.y = static_cast<float>(i) / static_cast<float>(numStacks);

				// Calculate normal as a normalized vertex position
				DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.pos));
				DirectX::XMStoreFloat3(&vertex.n, normal);

				vertices.push_back(vertex);
			}
		}

		// Generate indices
		for (int i = 0; i < numStacks; ++i)
		{
			for (int j = 0; j < numSlices; ++j)
			{
				int vertexIndex = i * (numSlices + 1) + j;

				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + 1);
				indices.push_back(vertexIndex + numSlices + 2);

				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + numSlices + 2);
				indices.push_back(vertexIndex + numSlices + 1);
			}
		}

		return { std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(12, 24);
	}
};