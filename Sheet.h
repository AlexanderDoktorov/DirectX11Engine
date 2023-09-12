#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Polygon.h"
#include "Interfaces.h"

class Sheet : public DrawableBase<Sheet>, public IColored, public IMovable, public IScalable, public IObject
{
public:
	Sheet(Graphics& Gfx, dx::XMFLOAT4 color = { 1.f,1.f,1.f,1.f } ) 
	{
		SetColor(color);
		if (!Initilized())
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			struct Vertex
			{
				dx::XMFLOAT3 pos;
				dx::XMFLOAT3 n;
			};

			auto model = Polygon::Make<Vertex>(4U, 100.f);
			model.SetNormalsIndependentFlat();

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryVS.cso");

			AddStaticElement(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticElement(std::make_unique<PixelShader>(Gfx, L"GeometryPS.cso"));
			AddStaticElement(std::make_unique<InputLayout>(Gfx, inputElementDesc, VS.get()));
			AddStaticElement(std::move(VS));
			AddStaticElement(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticElement(std::make_unique<IndexBuffer>(Gfx, model.indices));
		}
		AddElement(std::make_unique<TransformBuffer>(Gfx, *this));
		AddElement(std::make_unique<ColorBuffer>(Gfx, *this));
	}

	virtual void				Scale(float scale_x_new = 1.f, float scale_y_new = 1.f, float scale_z_new = 1.f)
	{
		scale_x = scale_x_new; scale_y = scale_y_new; scale_z = scale_z_new;
	}
	virtual DirectX::XMMATRIX	GetTransform()				const noexcept override
	{
		return DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw,  Roll) * DirectX::XMMatrixScaling(scale_x, scale_y, scale_z) * DirectX::XMMatrixTranslation(x, y, z);
	}

	// IMovable
	virtual void SetPosition(float _x, float _y, float _z) override;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

	// IColored
	DirectX::XMFLOAT4 GetColor() const noexcept override;
	void SetColor(dx::XMFLOAT4 new_color) noexcept override;

	// IScalable
	virtual void SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new) override;
	virtual dx::XMFLOAT3 GetScale() const noexcept override;

private:
	dx::XMFLOAT4 color = { 1.f,1.f,1.f,1.f };

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float scale_x = 1.f;
	float scale_y = 1.f;
	float scale_z = 1.f;

	float Roll = PI / 4.f;
	float Pitch = PI / 2.f;
	float Yaw = 0.f;
};