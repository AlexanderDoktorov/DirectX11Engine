#pragma once
#include <d3d11.h>

struct ISlot
{
	virtual void SetBindSlot(UINT slot) noexcept = 0;
	virtual UINT GetBindSlot() const noexcept = 0;
};