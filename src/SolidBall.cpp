#include "SolidBall.h"

void SolidBall::SetPosition(float _x, float _y, float _z)
{
    world_position.x = _x;
    world_position.y = _y;
    world_position.z = _z;
}

DirectX::XMFLOAT3 SolidBall::GetPosition() const noexcept
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
