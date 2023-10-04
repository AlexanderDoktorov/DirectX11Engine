#pragma once
#include "Interfaces.h"
#include "DOK_DX11.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Mesh.h"

class Character : public DrawableBase<Character>,  public IToString, public IScalable
{
public:
	Character(Graphics& Gfx);

	// IToString
	const char* ToString() const noexcept override;

	// IObject
	DirectX::XMMATRIX GetTransform() const noexcept override;

	// IScalable
	dx::XMFLOAT3& GetScaleRef() noexcept override;
private:
	dx::XMFLOAT3 pos	{ 0.f,0.f,0.f };
	dx::XMFLOAT3 scale	{ 1.f,1.f,1.f };
};