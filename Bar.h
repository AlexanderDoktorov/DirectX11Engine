#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Interfaces.h"
#include "Sampler.h"
#include "Cube.h"
#include "Vertex.h"

class Bar : public DrawableBase<Bar>, public IMovable, public IScalable, public IColored, public IToString
{
public:
	Bar(Graphics& Gfx, float length = 1.f, float height = 1.f, float width = 1.f) : scale(length, height, width)
	{
		if (!Initilized())
		{
			DynamicVertex::VertexLayout layout{};
			layout.Append(DynamicVertex::VertexLayout::Position3D).Append(DynamicVertex::VertexLayout::Normal).Append(DynamicVertex::VertexLayout::Texture2D);
			float side = 0.5f;

			auto model = Cube::Make(layout);;
			model.SetNormalsSmooth();

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryVS.cso");

			AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"GeometryPS.cso"));
			AddStaticBindable(std::make_unique<InputLayout>(Gfx, model.vertices.GetLayout().GetD3DLayout(), VS.get()));
			AddStaticBindable(std::move(VS));
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
			AddStaticBindable(std::make_unique<Sampler>(Gfx));
			//AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\Текстуры\\Текстура_квадрата.png", 3U));
		}
		AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
		AddBindable(std::make_unique<ColorBuffer>(Gfx, *this));
	}

	virtual DirectX::XMMATRIX	GetTransform()				const noexcept override
	{
		return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixTranslation(x, y, z);
	}

	// IMovable
	virtual void SetPosition(float _x, float _y, float _z) override;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

	// IScalable
	virtual dx::XMFLOAT3& GetScaleRef() noexcept { return scale; }

	// IColored
	virtual DirectX::XMFLOAT4 GetColor()	const noexcept override;
	virtual void SetColor(dx::XMFLOAT4 new_color) noexcept override;

	// IToString
	virtual const char* ToString() const noexcept override;

private:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	DirectX::XMFLOAT4 color { 1.f,0.f,1.f,1.f };
	DirectX::XMFLOAT3 scale { 1.f,1.f,1.f };
};