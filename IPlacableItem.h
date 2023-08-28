#pragma once
#include "DOK_DX11.h"
#include "DrawableBase.h"

class IPlacableItem
{
public:
	IPlacableItem() = default;

	virtual void SetWorldPosition(const dx::XMFLOAT3& new_Wpos) = 0;
	virtual dx::XMFLOAT3 GetWorldPosition() const noexcept = 0;

	virtual ~IPlacableItem() = default;
};
