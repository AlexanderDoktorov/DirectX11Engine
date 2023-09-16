#pragma once
#include <Windows.h>
#include "IndexedTriangleList.h"

class Cube
{
public:

	static IndexedTriangleList Make(Vertex::VertexLayout layout)
	{
		namespace dx = DirectX;
		Vertex::VertexBuffer vb{ std::move( layout ) };

		constexpr float side = 1.0f / 2.0f;

		vb.EmplaceBack(dx::XMFLOAT3({ -side,-side,-side }));
		vb.EmplaceBack(dx::XMFLOAT3({ side,-side,-side }));
		vb.EmplaceBack(dx::XMFLOAT3({ -side,side,-side }));
		vb.EmplaceBack(dx::XMFLOAT3({ side,side,-side }));
		vb.EmplaceBack(dx::XMFLOAT3({ -side,-side,side }));
		vb.EmplaceBack(dx::XMFLOAT3({ side,-side,side }));
		vb.EmplaceBack(dx::XMFLOAT3({ -side,side,side }));
		vb.EmplaceBack(dx::XMFLOAT3({ side,side,side }));

		return{
			std::move(vb),{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}

	static IndexedTriangleList MakeIndependent(Vertex::VertexLayout layout)
	{
		float side = 0.5f;

		Vertex::VertexBuffer vb{ std::move( layout ), 24U };

		vb[0].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,-side	});// 0 near side
		vb[1].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,-side	});// 1
		vb[2].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,-side	});// 2
		vb[3].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,-side		});// 3
		vb[4].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,side	});// 4 far side
		vb[5].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,side		});// 5
		vb[6].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,side		});// 6
		vb[7].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,side		});// 7
		vb[8].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,-side	});// 8 left side
		vb[9].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,-side	});// 9
		vb[10].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,side	});// 10
		vb[11].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,side	});// 11
		vb[12].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,-side	});// 12 right side
		vb[13].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,-side	});// 13
		vb[14].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,side	});// 14
		vb[15].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,side		});// 15
		vb[16].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,-side	});// 16 bottom side
		vb[17].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,-side	});// 17
		vb[18].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,side	});// 18
		vb[19].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,side	});// 19
		vb[20].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,-side	});// 20 top side
		vb[21].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,-side	});// 21
		vb[22].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,side	});// 22
		vb[23].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,side		});// 23

		return{
			std::move(vb),
			{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}

	static IndexedTriangleList MakeIndependentTextured()
	{
		using Type = Vertex::VertexLayout::ElementType;
	
		auto itl = MakeIndependent( std::move( Vertex::VertexLayout{}
			.Append( Vertex::VertexLayout::Position3D )
			.Append( Vertex::VertexLayout::Normal )
			.Append( Vertex::VertexLayout::Texture2D )
		) );
		
		itl.vertices[0].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[1].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[2].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[3].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[4].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[5].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[6].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[7].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[8].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[9].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[10].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[11].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[12].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[13].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[14].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[15].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[16].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[17].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[18].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[19].Attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[20].Attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[21].Attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[22].Attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[23].Attr<Type::Texture2D>() = { 1.0f,1.0f };

		return itl;
	}

	
};
