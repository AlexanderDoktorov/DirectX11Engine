#include "SolidBall.h"

void SolidBall::SetWorldPosition(const dx::XMFLOAT3& new_Wpos)
{
    world_position = new_Wpos;
}

dx::XMFLOAT3 SolidBall::GetWorldPosition() const noexcept
{
    return world_position;
}

const char* SolidBall::ToString() const noexcept
{
    return "Solid Ball";
}
