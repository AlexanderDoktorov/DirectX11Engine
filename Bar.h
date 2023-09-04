#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Cube.h"

class Bar : public DrawableBase<Bar>
{
public:
	Bar(Graphics& Gfx, float length = 4.f, float height = 1.f, float width = 1.f) : scale_x(length), scale_y(height), scale_z(width)
	{
		if (!Initilized())
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			struct Vertex
			{
				dx::XMFLOAT3 pos;
				dx::XMFLOAT2 tc;
				dx::XMFLOAT3 n;
			};

			auto model = Cube::MakeIndependentTextured<Vertex>();
			model.SetNormalsIndependentFlat();

			std::unique_ptr<VertexShader> VS = std::make_unique<VertexShader>(Gfx, L"GeometryTexturedVS.cso");
			auto ShBC = VS->GetBlob();

			AddStaticElement(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticElement(std::make_unique<PixelShader>(Gfx, L"GeometryTexturedPS.cso"));
			AddStaticElement(std::move(VS));
			AddStaticElement(std::make_unique<InputLayout>(Gfx, inputElementDesc, ShBC));
			AddStaticElement(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticElement(std::make_unique<IndexBuffer>(Gfx, model.indices));
			AddStaticElement(std::make_unique<Sampler>(Gfx));
			AddStaticElement(std::make_unique<Texture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\Текстуры\\Текстура_квадрата.png"));
		}
		AddElement(std::make_unique<TransformBuffer>(Gfx, *this));
	}

	void						SetPosition(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}

	virtual void				Scale(float scale_x_new = 1.f, float scale_y_new = 1.f, float scale_z_new = 1.f)
	{
		scale_x = scale_x_new; scale_y = scale_y_new; scale_z = scale_z_new;
	}
	virtual DirectX::XMMATRIX	GetTransform()				const noexcept override
	{
		return DirectX::XMMatrixScaling(scale_x, scale_y, scale_z) * DirectX::XMMatrixTranslation(x, y, z);
	}
	virtual void				Update(float dt)			noexcept override
	{
		return;
	}

private:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float scale_x = 1.f;
	float scale_y = 1.f;
	float scale_z = 1.f;
};