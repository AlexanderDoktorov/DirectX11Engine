#include "ISlot.h"

void Slotted::SetBindSlot(UINT slot) noexcept
{
	numSlot = slot;
}

UINT Slotted::GetBindSlot() const noexcept
{
	return numSlot;
}
