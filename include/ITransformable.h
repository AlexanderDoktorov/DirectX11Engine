#pragma once
#include <DirectXMath.h>

class ITransformable
{
public:
	virtual ~ITransformable() = default;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
};