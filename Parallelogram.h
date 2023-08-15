#pragma once
#include <Windows.h>
#include "DrawableBase.h"
#include "HPipelineElements.h"

template <float width, float height, float length>
class Parallelogram : DrawableBase< Parallelogram< width,height,length > >
{
public:
	Parallelogram(Graphics& Gfx)
	{
		DOK_assert(Initilized(), L"Make sure you made template of Parallelogram with Parallelogram::Make(Graphics& gfx)");

		AddElement(std::make_unique<ColorBuffer>(Gfx, *this));
		AddElement(std::make_unique<TransformBuffer>(Gfx, *this));
	}

	static void Make(Graphics& gfx)
	{
		if (!Initilized())
		{
			auto half_x = length / 2.f;
			auto half_y = height / 2.f;
			auto half_z = width / 2.f;

			struct Vertex
			{
				dx::XMFLOAT3 position;
				dx::XMFLOAT2 textcoord;
			};

			std::vector<Vertex> vertecies =
			{
				{ dx::XMFLOAT3(-half_x, half_y, -half_z),       dx::XMFLOAT2(0.f, 0.f) },
				{ dx::XMFLOAT3(half_x, half_y, -half_z),        dx::XMFLOAT2(1.f, 0.f) },
				{ dx::XMFLOAT3(-half_x, -half_y, -half_z),      dx::XMFLOAT2(0.f, 1.f) },
				{ dx::XMFLOAT3(half_x, -half_y, -half_z),       dx::XMFLOAT2(1.f, 1.f) },

				{ dx::XMFLOAT3(-half_x, half_y, half_z),        dx::XMFLOAT2(1.f, 0.f) },
				{ dx::XMFLOAT3(half_x, half_y, half_z),         dx::XMFLOAT2(0.f, 0.f) },
				{ dx::XMFLOAT3(-half_x, -half_y, half_z),       dx::XMFLOAT2(1.f, 1.f) },
				{ dx::XMFLOAT3(half_x, -half_y, half_z),        dx::XMFLOAT2(0.f, 1.f) }
			};

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			std::vector<unsigned short>  indicies =
			{
				2,0,3, 0,1,3, // Front
				2,6,0, 4,0,6, // Left
				3,1,7, 1,5,7, // Right
				3,6,2, 7,6,3, // Down
				0,4,1, 1,4,5, // Up
				7,4,6, 5,4,7  // Back
			};

			std::unique_ptr<VertexShader> VS = std::make_unique<VertexShader>(Gfx, L"ParallelogramVertexShader.cso");
			auto ShBC = VS->GetBlob();

			AddStaticElement(std::make_unique<VertexBuffer>(Gfx, vertecies));
			AddStaticElement(std::make_unique<PixelShader>(Gfx, L"ParallelogramPixelShader.cso"));
			AddStaticElement(std::move(VS));
			AddStaticElement(std::make_unique<InputLayout>(Gfx, inputElementDesc, ShBC));
			AddStaticElement(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticElement(std::make_unique<IndexBuffer>(Gfx, indicies));
			AddStaticElement(std::make_unique<Sampler>(Gfx));
			AddStaticElement(std::make_unique<Texture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\Текстуры\\Текстура_квадрата.png"));
		}
	}

	void						SetPosition(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}
	virtual DirectX::XMMATRIX	GetTransform()				const noexcept override
	{
		return DirectX::XMMatrixTranslation(x, y, z);
	}
	virtual void				Update(float dt)			noexcept override
	{
		return;
	}
private:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};

Parallelogram<1.f,1.f,1.f> plg;
