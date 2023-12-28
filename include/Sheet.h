#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "../include/FlatRect.h"
#include "Interfaces.h"
#include "Mesh.h"

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
			vertexLayout.Append(Position3D).Append(Texture2D).Append(Normal).Append(Tangent).Append(Bitangent);

			auto model = FlatRect::MakeNormalizedTexturedTBN(vertexLayout);

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\NormalTextureVS.cso");

			AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticBindable(std::make_unique<PixelShader>(Gfx, L"shaders\\NormalTexturePS.cso"));
			AddStaticBindable(std::make_unique<InputLayout>(Gfx, vertexLayout, VS.get()));
			AddStaticBindable(std::move(VS));
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBindable(std::make_unique<WICTexture>(Gfx,  "Models\\brick_wall\\brick_wall_diffuse.jpg", SLOT_TEXTURE_DIFFUSE));
			AddStaticBindable(std::make_unique<WICTexture>(Gfx,  "Models\\brick_wall\\brick_wall_normal.jpg", SLOT_TEXTURE_NORMALMAP));
			AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
			AddStaticBindable(std::make_unique<Sampler>(Gfx));
		}
		AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
		AddBindable(std::make_unique<TransformBufferPS>(Gfx, *this));
		AddBindable(std::make_unique<DataBufferPS<DATA, 2U>>(Gfx, data));
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
			ImGui::SliderAngle("Roll", &Roll);
			ImGui::SliderAngle("Pitch", &Pitch);
			ImGui::SliderAngle("Yaw", &Yaw);
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
	virtual void SetScaleX(const float& scale_x_new) override;
	virtual void SetScaleY(const float& scale_y_new) override;
	virtual void SetScaleZ(const float& scale_z_new) override;
	virtual dx::XMFLOAT3 GetScale() const noexcept	 override;

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

	float Roll = dx::XM_PI / 4.f;
	float Pitch = dx::XM_PI / 2.f;
	float Yaw = 0.f;
};