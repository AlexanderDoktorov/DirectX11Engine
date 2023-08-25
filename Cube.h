#pragma once
#include <Windows.h>
#include "IndexedTriangleList.h"

class Cube
{
public:

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		namespace dx = DirectX;

		constexpr float side = 1.0f / 2.0f;

		std::vector<V> vertices(8);
		vertices[0].pos = { -side,-side,-side };
		vertices[1].pos = { side,-side,-side };
		vertices[2].pos = { -side,side,-side };
		vertices[3].pos = { side,side,-side };
		vertices[4].pos = { -side,-side,side };
		vertices[5].pos = { side,-side,side };
		vertices[6].pos = { -side,side,side };
		vertices[7].pos = { side,side,side };

		return{
			std::move(vertices),{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}

	template <class V>
	static IndexedTriangleList<V> MakeIndependent()
	{
		float side = 0.5f;

		std::vector<V> vertices(4U * 6U);

		vertices[0].pos = { -side,-side,-side };// 0 near side
		vertices[1].pos = { side,-side,-side };// 1
		vertices[2].pos = { -side,side,-side };// 2
		vertices[3].pos = { side,side,-side };// 3
		vertices[4].pos = { -side,-side,side };// 4 far side
		vertices[5].pos = { side,-side,side };// 5
		vertices[6].pos = { -side,side,side };// 6
		vertices[7].pos = { side,side,side };// 7
		vertices[8].pos = { -side,-side,-side };// 8 left side
		vertices[9].pos = { -side,side,-side };// 9
		vertices[10].pos = { -side,-side,side };// 10
		vertices[11].pos = { -side,side,side };// 11
		vertices[12].pos = { side,-side,-side };// 12 right side
		vertices[13].pos = { side,side,-side };// 13
		vertices[14].pos = { side,-side,side };// 14
		vertices[15].pos = { side,side,side };// 15
		vertices[16].pos = { -side,-side,-side };// 16 bottom side
		vertices[17].pos = { side,-side,-side };// 17
		vertices[18].pos = { -side,-side,side };// 18
		vertices[19].pos = { side,-side,side };// 19
		vertices[20].pos = { -side,side,-side };// 20 top side
		vertices[21].pos = { side,side,-side };// 21
		vertices[22].pos = { -side,side,side };// 22
		vertices[23].pos = { side,side,side };// 23

		return{
			std::move(vertices),
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

	template<class V>
	static IndexedTriangleList<V> MakeIndependentTextured()
	{
		auto itl = MakeIndependent<V>();

		itl.vertices[0].tc = { 0.0f,0.0f };
		itl.vertices[1].tc = { 1.0f,0.0f };
		itl.vertices[2].tc = { 0.0f,1.0f };
		itl.vertices[3].tc = { 1.0f,1.0f };
		itl.vertices[4].tc = { 0.0f,0.0f };
		itl.vertices[5].tc = { 1.0f,0.0f };
		itl.vertices[6].tc = { 0.0f,1.0f };
		itl.vertices[7].tc = { 1.0f,1.0f };
		itl.vertices[8].tc = { 0.0f,0.0f };
		itl.vertices[9].tc = { 1.0f,0.0f };
		itl.vertices[10].tc = { 0.0f,1.0f };
		itl.vertices[11].tc = { 1.0f,1.0f };
		itl.vertices[12].tc = { 0.0f,0.0f };
		itl.vertices[13].tc = { 1.0f,0.0f };
		itl.vertices[14].tc = { 0.0f,1.0f };
		itl.vertices[15].tc = { 1.0f,1.0f };
		itl.vertices[16].tc = { 0.0f,0.0f };
		itl.vertices[17].tc = { 1.0f,0.0f };
		itl.vertices[18].tc = { 0.0f,1.0f };
		itl.vertices[19].tc = { 1.0f,1.0f };
		itl.vertices[20].tc = { 0.0f,0.0f };
		itl.vertices[21].tc = { 1.0f,0.0f };
		itl.vertices[22].tc = { 0.0f,1.0f };
		itl.vertices[23].tc = { 1.0f,1.0f };

		return itl;
	}

	
};
