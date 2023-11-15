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
			using enum DynamicVertex::VertexLayout::ElementType;
			DynamicVertex::VertexLayout vertexLayout;
			vertexLayout.Append(Position3D).Append(Texture2D).Append(Normal);

			auto model = Polygon::MakeTexturedNormalized(vertexLayout, 4U, 1.f);

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\GeometryTexturedVS.cso");

			AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticBindable(std::make_unique<PixelShader>(Gfx, L"shaders\\NormalTexturePS.cso"));
			AddStaticBindable(std::make_unique<InputLayout>(Gfx, vertexLayout.GetD3DLayout(), VS.get()));
			AddStaticBindable(std::move(VS));
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx,  L"Textures\\Brick_Wall_017_basecolor.jpg", 3U));
			AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx,  L"Textures\\Brick_Wall_017_normal.jpg", 4U));
			AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
			AddStaticBindable(std::make_unique<Sampler>(Gfx));
		}
		AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
		AddBindable(std::make_unique<TransformBufferPS>(Gfx, *this));
		AddBindable(std::make_unique<DataBufferPS<DATA>>(Gfx, data, 2U));
		// AddBindable(std::make_unique<ColorBuffer>(Gfx, *this));
	}

	virtual void				Scale(float scale_x_new = 1.f, float scale_y_new = 1.f, float scale_z_new = 1.f)
	{
		scale_x = scale_x_new; scale_y = scale_y_new; scale_z = scale_z_new;
	}
	virtual DirectX::XMMATRIX	GetTransform()				const noexcept override
	{
		return DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw,  Roll) * DirectX::XMMatrixScaling(scale_x, scale_y, scale_z) * DirectX::XMMatrixTranslation(x, y, z);
	}

	void ShowControlWindow() noexcept
	{
		if (ImGui::Begin("Sheet Options"))
		{
			ImGui::Checkbox("Enable normal map",  &data.normalMapEnabled);
		}
		ImGui::End();
	}

	// IMovable
	virtual void SetPosition(float _x, float _y, float _z) override;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

	// IColored
	DirectX::XMFLOAT4 GetColor() const noexcept override;
	void SetColor(dx::XMFLOAT4 new_color) noexcept override;

	// IScalable
	virtual dx::XMFLOAT3& GetScaleRef() noexcept { return scale; }

private:
	dx::XMFLOAT4 color { 1.f,1.f,1.f,1.f };
	dx::XMFLOAT3 scale { 1.f,1.f,1.f };
	
	struct DATA
	{
		alignas (16) bool normalMapEnabled = true;
	} data;

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float scale_x = 1.f;
	float scale_y = 1.f;
	float scale_z = 1.f;

	float Roll = 0; // PI / 4.f;
	float Pitch = 0; // PI / 2.f;
	float Yaw = 0.f;
};