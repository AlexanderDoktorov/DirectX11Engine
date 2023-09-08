#include "SolidLightenedBall.h"

const char* SolidLightenedBall::ToString() const noexcept
{
    return "Solid Lightened Ball";
}

DirectX::XMFLOAT4 SolidLightenedBall::GetColor() const noexcept
{
    return color;
}

void SolidLightenedBall::SetColor(dx::XMFLOAT4 new_color) noexcept
{
    color = new_color;
}

void SolidLightenedBall::SetWorldPosition(const dx::XMFLOAT3& new_Wpos)
{
    world_position = new_Wpos;
}

dx::XMFLOAT3 SolidLightenedBall::GetWorldPosition() const noexcept
{
    return world_position;
}
