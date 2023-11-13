#pragma once

class IObject
{
public:
	virtual ~IObject() = default;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
};