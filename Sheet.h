#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Polygon.h"
#include "Interfaces.h"

class Sheet : public DrawableBase<Sheet>, public IColored, public IMovable, public IScalable
{
public:
	Sheet(Graphics& Gfx, dx::XMFLOAT4 color = { 1.f,1.f,1.f,1.f } ) 
	{
		SetColor(color);
		if (!Initilized())
		{
			Vertex::VertexLayout vertexLayout;
			vertexLayout.Append(Vertex::VertexLayout::Position3D).Append(Vertex::VertexLayout::Normal);

			auto model = Polygon::Make(vertexLayout, 4U, 100.f);

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryVS.cso");

			AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticBindable(std::make_unique<PixelShader>(Gfx, L"GeometryPS.cso"));
			AddStaticBindable(std::make_unique<InputLayout>(Gfx, vertexLayout.GetD3DLayout(), VS.get()));
			AddStaticBindable(std::move(VS));
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
		}
		AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
		AddBindable(std::make_unique<ColorBuffer>(Gfx, *this));
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