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

DirectX::XMFLOAT4 SolidBall::GetColor() const noexcept
{
    return color;
}

void SolidBall::SetColor(dx::XMFLOAT4 new_color) noexcept
{
    color = new_color;
}
