#pragma once
#include <d3d11.h>

struct ISlot
{
	virtual ~ISlot() = default;
	virtual void SetBindSlot(UINT slot) noexcept = 0;
	virtual UINT GetBindSlot() const noexcept = 0;
};

struct Slotted : public ISlot
{
	Slotted(UINT numSlot) : numSlot(numSlot)
	{}
	Slotted() : numSlot(0U)
	{}
	void SetBindSlot(UINT slot) noexcept override;
	UINT GetBindSlot() const noexcept override;
	virtual ~Slotted() = default;
private:
	UINT numSlot = 0U;
};