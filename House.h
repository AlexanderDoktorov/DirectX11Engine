#pragma once
#include "Interfaces.h"
#include "DOK_DX11.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Mesh.h"

class House : public DrawableBase<House>,  public IToString, public IScalable
{
public:
	House(Graphics& Gfx);

	// IToString
	const char* ToString() const noexcept override;

	// IObject
	DirectX::XMMATRIX GetTransform() const noexcept override;

	// IScalable
	virtual dx::XMFLOAT3 GetScale() const noexcept override;
	virtual void SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new) override;
private:
	dx::XMFLOAT3 pos{ 0.f,0.f,0.f };
	dx::XMFLOAT3 scale{ 1.f,1.f,1.f };
};