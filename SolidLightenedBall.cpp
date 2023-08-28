#include "SolidLightenedBall.h"

const char* SolidLightenedBall::ToString() const noexcept
{
    return "Solid Lightened Ball";
}

void SolidLightenedBall::SetWorldPosition(const dx::XMFLOAT3& new_Wpos)
{
    world_position = new_Wpos;
}

dx::XMFLOAT3 SolidLightenedBall::GetWorldPosition() const noexcept
{
    return world_position;
}
