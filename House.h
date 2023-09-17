#pragma once
#include "Interfaces.h"
#include "DOK_DX11.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/Scene.h"

class House : public DrawableBase<House>,  public IToString
{
public:
	House(Graphics& Gfx);

	// IToString
	const char* ToString() const noexcept override;

	// IObject
	DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	dx::XMFLOAT3 pos{ 0.f,0.f,0.f };
};