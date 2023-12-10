#pragma once
#include <DirectXMath.h>

class IObject
{
public:
	virtual ~IObject() = default;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
};