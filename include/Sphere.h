#pragma once
#include "IndexedTriangleList.h"
#include "DOK_math.h"
class Sphere
{
public:
	static IndexedTriangleList MakeTesselated( DynamicVertex::VertexLayout layout,int latDiv,int longDiv )
	{
		namespace dx = DirectX;
		assert( latDiv >= 3 );
		assert( longDiv >= 3 );

		constexpr float radius = 1.0f;
		const auto base = dx::XMVectorSet( 0.0f,0.0f,radius,0.0f );
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		DynamicVertex::VertexBuffer vb{ std::move( layout ) };
		for( int iLat = 1; iLat < latDiv; iLat++ )
		{
			const auto latBase = dx::XMVector3Transform( 
				base,
				dx::XMMatrixRotationX( lattitudeAngle * iLat ) 
			);
			for( int iLong = 0; iLong < longDiv; iLong++ )
			{
				dx::XMFLOAT3 calculatedPos;
				auto v = dx::XMVector3Transform( 
					latBase,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				dx::XMStoreFloat3( &calculatedPos,v );
				vb.EmplaceBack( calculatedPos );
			}
		}

		// add the cap vertices
		const auto iNorthPole = (unsigned short)vb.Size();
		{
			dx::XMFLOAT3 northPos;
			dx::XMStoreFloat3( &northPos,base );
			vb.EmplaceBack( northPos );
		}
		const auto iSouthPole = (unsigned short)vb.Size();
		{
			dx::XMFLOAT3 southPos;
			dx::XMStoreFloat3( &southPos,dx::XMVectorNegate( base ) );
			vb.EmplaceBack( southPos );
		}

		const auto calcIdx = [latDiv,longDiv]( unsigned short iLat,unsigned short iLong )
			{ return iLat * longDiv + iLong; };
		std::vector<unsigned short> indices;
		for( unsigned short iLat = 0; iLat < latDiv - 2; iLat++ )
		{
			for( unsigned short iLong = 0; iLong < longDiv - 1; iLong++ )
			{
				indices.push_back( calcIdx( iLat,iLong ) );
				indices.push_back( calcIdx( iLat + 1,iLong ) );
				indices.push_back( calcIdx( iLat,iLong + 1 ) );
				indices.push_back( calcIdx( iLat,iLong + 1 ) );
				indices.push_back( calcIdx( iLat + 1,iLong ) );
				indices.push_back( calcIdx( iLat + 1,iLong + 1 ) );
			}
			// wrap band
			indices.push_back( calcIdx( iLat,longDiv - 1 ) );
			indices.push_back( calcIdx( iLat + 1,longDiv - 1 ) );
			indices.push_back( calcIdx( iLat,0 ) );
			indices.push_back( calcIdx( iLat,0 ) );
			indices.push_back( calcIdx( iLat + 1,longDiv - 1 ) );
			indices.push_back( calcIdx( iLat + 1,0 ) );			
		}

		// cap fans
		for( unsigned short iLong = 0; iLong < longDiv - 1; iLong++ )
		{
			// north
			indices.push_back( iNorthPole );
			indices.push_back( calcIdx( 0,iLong ) );
			indices.push_back( calcIdx( 0,iLong + 1 ) );
			// south
			indices.push_back( calcIdx( latDiv - 2,iLong + 1 ) );
			indices.push_back( calcIdx( latDiv - 2,iLong ) );
			indices.push_back( iSouthPole );
		}
		// wrap triangles
		// north
		indices.push_back( iNorthPole );
		indices.push_back( calcIdx( 0,longDiv - 1 ) );
		indices.push_back( calcIdx( 0,0 ) );
		// south
		indices.push_back( calcIdx( latDiv - 2,0 ) );
		indices.push_back( calcIdx( latDiv - 2,longDiv - 1 ) );
		indices.push_back( iSouthPole );

		return { std::move( vb ),std::move( indices ) };
	}

	static IndexedTriangleList MakeTesselatedIndependentTexturedNormalized(DynamicVertex::VertexLayout layout, int numSlices, int numStacks, float radius)
	{
		DynamicVertex::VertexBuffer vb{ std::move( layout ) };

		std::vector<unsigned short> indices;

		for (int i = 0; i <= numStacks; ++i)
		{
			float phi = DirectX::XM_PI * static_cast<float>(i) / static_cast<float>(numStacks);
			for (int j = 0; j <= numSlices; ++j)
			{
				float theta = DirectX::XM_2PI * static_cast<float>(j) / static_cast<float>(numSlices);

				dx::XMFLOAT3 pos{};
				pos.x = radius * sinf(phi) * cosf(theta);
				pos.y = radius * cosf(phi);
				pos.z = radius * sinf(phi) * sinf(theta);
				auto tcx = static_cast<float>(j) / static_cast<float>(numSlices);
				auto tcy = static_cast<float>(i) / static_cast<float>(numStacks);

				// Calculate normal as a normalized vertex position
				dx::XMFLOAT3 n;
				DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pos));
				DirectX::XMStoreFloat3(&n, normal);

				vb.EmplaceBack(pos, dx::XMFLOAT2(tcx,tcy), n);
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

		return { std::move(vb),std::move(indices) };
	}
};