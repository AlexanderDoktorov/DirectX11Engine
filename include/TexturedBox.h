#pragma once
#include "HPipelineElements.h"
#include "DrawableBase.h"
#include "Interfaces.h"
#include "Cube.h"
#include "Materials.h"

class TexturedBox : public DrawableBase<TexturedBox>, public IMovable, public IScalable, public IToString
{
public:
	TexturedBox(Graphics& Gfx)
	{
		if (!Initilized())
		{
			auto model = Cube::MakeIndependentTexturedTBN();

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\NormalTextureVS.cso");

			AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"shaders\\NormalTexturePS.cso"));
			AddStaticBindable(std::make_unique<InputLayout>(Gfx, model.vertices.GetLayout().GetD3DLayout(), VS.get()));
			AddStaticBindable(std::move(VS));
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
			AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx, L"Textures\\Brick_Wall_017_basecolor.jpg", SLOT_TEXTURE_DIFFUSE));
			AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx, L"Textures\\Brick_Wall_017_normal.jpg", SLOT_TEXTURE_NORMALMAP));
			AddStaticBindable(std::make_unique<Sampler>(Gfx));
		}
		AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
	}

	// IObject
	virtual DirectX::XMMATRIX	GetTransform() const noexcept override;

	// IMovable
	virtual void SetPosition(float _x, float _y, float _z) override;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

	// IScalable
	virtual void SetScaleX(const float& scale_x_new) override;
	virtual void SetScaleY(const float& scale_y_new) override;
	virtual void SetScaleZ(const float& scale_z_new) override;
	virtual dx::XMFLOAT3 GetScale() const noexcept	 override;

	// IToString
	virtual const char* ToString() const noexcept override;

private:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	DirectX::XMFLOAT3 scale { 3.f, 3.f, 3.f };
};