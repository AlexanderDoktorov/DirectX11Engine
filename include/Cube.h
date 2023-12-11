#pragma once
#include <Windows.h>
#include "IndexedTriangleList.h"

class Cube
{
public:

	static IndexedTriangleList Make(DynamicVertex::VertexLayout layout)
	{
		using enum DynamicVertex::VertexLayout::ElementType;
		namespace dx = DirectX;
		assert(layout.HasSuchElement<Position3D>());
		DynamicVertex::VertexBuffer vb{ std::move( layout ), 8U };

		constexpr float side = 1.0f / 2.0f;

		vb[0].Attr<Position3D>() = (dx::XMFLOAT3({ -side,-side,-side }));
		vb[1].Attr<Position3D>() = (dx::XMFLOAT3({ side,-side,-side }));
		vb[2].Attr<Position3D>() = (dx::XMFLOAT3({ -side,side,-side }));
		vb[3].Attr<Position3D>() = (dx::XMFLOAT3({ side,side,-side }));
		vb[4].Attr<Position3D>() = (dx::XMFLOAT3({ -side,-side,side }));
		vb[5].Attr<Position3D>() = (dx::XMFLOAT3({ side,-side,side }));
		vb[6].Attr<Position3D>() = (dx::XMFLOAT3({ -side,side,side }));
		vb[7].Attr<Position3D>() = (dx::XMFLOAT3({ side,side,side }));

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

	static IndexedTriangleList MakeIndependent(DynamicVertex::VertexLayout layout)
	{
		float side = 0.5f;

		DynamicVertex::VertexBuffer vb{ std::move( layout ), 24U };

		vb[0].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,-side	});// 0 near side
		vb[1].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,-side	});// 1
		vb[2].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,-side	});// 2
		vb[3].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,-side		});// 3
		vb[4].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,side	});// 4 far side
		vb[5].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,side		});// 5
		vb[6].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,side		});// 6
		vb[7].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,side		});// 7
		vb[8].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,-side	});// 8 left side
		vb[9].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,-side	});// 9
		vb[10].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,side	});// 10
		vb[11].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,side	});// 11
		vb[12].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,-side	});// 12 right side
		vb[13].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,-side	});// 13
		vb[14].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,side	});// 14
		vb[15].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,side		});// 15
		vb[16].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,-side	});// 16 bottom side
		vb[17].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,-side	});// 17
		vb[18].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,side	});// 18
		vb[19].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,side	});// 19
		vb[20].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,-side	});// 20 top side
		vb[21].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,-side	});// 21
		vb[22].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,side	});// 22
		vb[23].Attr<DynamicVertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,side		});// 23

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
		using Type = DynamicVertex::VertexLayout::ElementType;
	
		auto itl = MakeIndependent( std::move( DynamicVertex::VertexLayout{}
			.Append( DynamicVertex::VertexLayout::Position3D )
			.Append( DynamicVertex::VertexLayout::Normal )
			.Append( DynamicVertex::VertexLayout::Texture2D )
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

	static IndexedTriangleList MakeIndependentTexturedTBN()
	{
		using enum DynamicVertex::VertexLayout::ElementType;
		auto vb = DynamicVertex::VertexBuffer(std::move( DynamicVertex::VertexLayout{}
			.Append( DynamicVertex::VertexLayout::Position3D )
			.Append( DynamicVertex::VertexLayout::Normal )
			.Append( DynamicVertex::VertexLayout::Texture2D )
			.Append( DynamicVertex::VertexLayout::Tangent )
			.Append( DynamicVertex::VertexLayout::Bitangent )
		), 24U);

		constexpr float side = 1.0f / 2.0f;
		constexpr dx::XMFLOAT3 positions[24U] =
		{
			dx::XMFLOAT3({ -side,-side,-side	}),// 0 near side
			dx::XMFLOAT3({ side,-side,-side		}),// 1
			dx::XMFLOAT3({ -side,side,-side		}),// 2
			dx::XMFLOAT3({ side,side,-side		}),// 3
			dx::XMFLOAT3({ -side,-side,side		}),// 4 far side
			dx::XMFLOAT3({ side,-side,side		}),// 5
			dx::XMFLOAT3({ -side,side,side		}),// 6
			dx::XMFLOAT3({ side,side,side		}),// 7
			dx::XMFLOAT3({ -side,-side,-side	}),// 8 left side
			dx::XMFLOAT3({ -side,side,-side		}),// 9
			dx::XMFLOAT3({ -side,-side,side		}),// 10
			dx::XMFLOAT3({ -side,side,side		}),// 11
			dx::XMFLOAT3({ side,-side,-side		}),// 12 right side
			dx::XMFLOAT3({ side,side,-side		}),// 13
			dx::XMFLOAT3({ side,-side,side		}),// 14
			dx::XMFLOAT3({ side,side,side		}),// 15
			dx::XMFLOAT3({ -side,-side,-side	}),// 16 bottom side
			dx::XMFLOAT3({ side,-side,-side		}),// 17
			dx::XMFLOAT3({ -side,-side,side		}),// 18
			dx::XMFLOAT3({ side,-side,side		}),// 19
			dx::XMFLOAT3({ -side,side,-side		}),// 20 top side
			dx::XMFLOAT3({ side,side,-side		}),// 21
			dx::XMFLOAT3({ -side,side,side		}),// 22
			dx::XMFLOAT3({ side,side,side		}), // 23
		};



		vb[0].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[0];
		vb[1].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[1];
		vb[2].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[2];
		vb[3].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[3];
		vb[4].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[4];
		vb[5].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[5];
		vb[6].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[6];
		vb[7].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[7];
		vb[8].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[8];
		vb[9].Attr<DynamicVertex::VertexLayout::Position3D>()  = positions[9];
		vb[10].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[10];
		vb[11].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[11];
		vb[12].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[12];
		vb[13].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[13];
		vb[14].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[14];
		vb[15].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[15];
		vb[16].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[16];
		vb[17].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[17];
		vb[18].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[18];
		vb[19].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[19];
		vb[20].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[20];
		vb[21].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[21];
		vb[22].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[22];
		vb[23].Attr<DynamicVertex::VertexLayout::Position3D>() = positions[23];

		constexpr dx::XMFLOAT2 textures[4] =
		{
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 0.0f,1.0f },
			{ 1.0f,1.0f },
		};

		vb[0].Attr<Texture2D>() = textures[0];
		vb[1].Attr<Texture2D>() = textures[1];
		vb[2].Attr<Texture2D>() = textures[2];
		vb[3].Attr<Texture2D>() = textures[3];
		vb[4].Attr<Texture2D>() = textures[0];
		vb[5].Attr<Texture2D>() = textures[1];
		vb[6].Attr<Texture2D>() = textures[2];
		vb[7].Attr<Texture2D>() = textures[3];
		vb[8].Attr<Texture2D>()  = textures[0];
		vb[9].Attr<Texture2D>()  = textures[1];
		vb[10].Attr<Texture2D>() = textures[2];
		vb[11].Attr<Texture2D>() = textures[3];
		vb[12].Attr<Texture2D>() = textures[0];
		vb[13].Attr<Texture2D>() = textures[1];
		vb[14].Attr<Texture2D>() = textures[2];
		vb[15].Attr<Texture2D>() = textures[3];
		vb[16].Attr<Texture2D>() = textures[0];
		vb[17].Attr<Texture2D>() = textures[1];
		vb[18].Attr<Texture2D>() = textures[2];
		vb[19].Attr<Texture2D>() = textures[3];
		vb[20].Attr<Texture2D>() = textures[0];
		vb[21].Attr<Texture2D>() = textures[1];
		vb[22].Attr<Texture2D>() = textures[2];
		vb[23].Attr<Texture2D>() = textures[3];

		constexpr dx::XMFLOAT3 NormalNear	= { 0.f,	0.f,	-1.f};
		constexpr dx::XMFLOAT3 NormalFar	= { 0.f,	0.f,	1.f};
		constexpr dx::XMFLOAT3 NormalLeft	= { -1.f,	0.f,	0.f};
		constexpr dx::XMFLOAT3 NormalRight	= { 1.f,	0.f,	0.f };
		constexpr dx::XMFLOAT3 NormalBottom = { 0.f,	-1.f,	0.f};
		constexpr dx::XMFLOAT3 NormalTop	= { 0.f,	1.f,	0.f};

		constexpr TanBitan tbNear	= { { 1.f, 0.f, 0.f }, { 0.f,  -1.f, 0.f } };
		constexpr TanBitan tbFar	= { { -1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } }; // same as near
		constexpr TanBitan tbLeft	= { { 0.f, 0.f, -1.f }, { 0.f, -1.f, 0.f } };
		constexpr TanBitan tbRight	= { { 0.f, 0.f, 1.f }, { 0.f,  -1.f, 0.f } };
		constexpr TanBitan tbBottom = { { 1.f, 0.f, 0.f }, { 0.f,  0.f, 1.f } };
		constexpr TanBitan tbTop	= { { 1.f, 0.f, 0.f }, { 0.f,  0.f, -1.f } };

		vb[0].Attr<Normal>() =  NormalNear;
		vb[1].Attr<Normal>() =  NormalNear;
		vb[2].Attr<Normal>() =  NormalNear;
		vb[3].Attr<Normal>() =  NormalNear;
		vb[4].Attr<Normal>() =  NormalFar;
		vb[5].Attr<Normal>() =  NormalFar;
		vb[6].Attr<Normal>() =  NormalFar;
		vb[7].Attr<Normal>() =  NormalFar;
		vb[8].Attr<Normal>() =  NormalLeft;
		vb[9].Attr<Normal>() =  NormalLeft;
		vb[10].Attr<Normal>() = NormalLeft;
		vb[11].Attr<Normal>() = NormalLeft;
		vb[12].Attr<Normal>() = NormalRight;
		vb[13].Attr<Normal>() = NormalRight;
		vb[14].Attr<Normal>() = NormalRight;
		vb[15].Attr<Normal>() = NormalRight;
		vb[16].Attr<Normal>() = NormalBottom;
		vb[17].Attr<Normal>() = NormalBottom;
		vb[18].Attr<Normal>() = NormalBottom;
		vb[19].Attr<Normal>() = NormalBottom;
		vb[20].Attr<Normal>() = NormalTop;
		vb[21].Attr<Normal>() = NormalTop;
		vb[22].Attr<Normal>() = NormalTop;
		vb[23].Attr<Normal>() = NormalTop;

		vb[0].Attr<Tangent>() =  tbNear.tan;
		vb[1].Attr<Tangent>() =  tbNear.tan;
		vb[2].Attr<Tangent>() =  tbNear.tan;
		vb[3].Attr<Tangent>() =  tbNear.tan;
		vb[4].Attr<Tangent>() =  tbFar.tan;
		vb[5].Attr<Tangent>() =  tbFar.tan;
		vb[6].Attr<Tangent>() =  tbFar.tan;
		vb[7].Attr<Tangent>() =  tbFar.tan;
		vb[8].Attr<Tangent>() =  tbLeft.tan;
		vb[9].Attr<Tangent>() =  tbLeft.tan;
		vb[10].Attr<Tangent>() = tbLeft.tan;
		vb[11].Attr<Tangent>() = tbLeft.tan;
		vb[12].Attr<Tangent>() = tbRight.tan;
		vb[13].Attr<Tangent>() = tbRight.tan;
		vb[14].Attr<Tangent>() = tbRight.tan;
		vb[15].Attr<Tangent>() = tbRight.tan;
		vb[16].Attr<Tangent>() = tbBottom.tan;
		vb[17].Attr<Tangent>() = tbBottom.tan;
		vb[18].Attr<Tangent>() = tbBottom.tan;
		vb[19].Attr<Tangent>() = tbBottom.tan;
		vb[20].Attr<Tangent>() = tbTop.tan;
		vb[21].Attr<Tangent>() = tbTop.tan;
		vb[22].Attr<Tangent>() = tbTop.tan;
		vb[23].Attr<Tangent>() = tbTop.tan;

		vb[0].Attr<Bitangent>() =  tbNear.bitan;
		vb[1].Attr<Bitangent>() =  tbNear.bitan;
		vb[2].Attr<Bitangent>() =  tbNear.bitan;
		vb[3].Attr<Bitangent>() =  tbNear.bitan;
		vb[4].Attr<Bitangent>() =  tbFar.bitan;
		vb[5].Attr<Bitangent>() =  tbFar.bitan;
		vb[6].Attr<Bitangent>() =  tbFar.bitan;
		vb[7].Attr<Bitangent>() =  tbFar.bitan;
		vb[8].Attr<Bitangent>() =  tbLeft.bitan;
		vb[9].Attr<Bitangent>() =  tbLeft.bitan;
		vb[10].Attr<Bitangent>() = tbLeft.bitan;
		vb[11].Attr<Bitangent>() = tbLeft.bitan;
		vb[12].Attr<Bitangent>() = tbRight.bitan;
		vb[13].Attr<Bitangent>() = tbRight.bitan;
		vb[14].Attr<Bitangent>() = tbRight.bitan;
		vb[15].Attr<Bitangent>() = tbRight.bitan;
		vb[16].Attr<Bitangent>() = tbBottom.bitan;
		vb[17].Attr<Bitangent>() = tbBottom.bitan;
		vb[18].Attr<Bitangent>() = tbBottom.bitan;
		vb[19].Attr<Bitangent>() = tbBottom.bitan;
		vb[20].Attr<Bitangent>() = tbTop.bitan;
		vb[21].Attr<Bitangent>() = tbTop.bitan;
		vb[22].Attr<Bitangent>() = tbTop.bitan;
		vb[23].Attr<Bitangent>() = tbTop.bitan;

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

	
};
