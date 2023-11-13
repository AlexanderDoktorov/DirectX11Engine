#include "SolidLightenedBall.h"

void SolidLightenedBall::SetPosition(float _x, float _y, float _z)
{
    world_position.x = _x;
    world_position.y = _y;
    world_position.z = _z;
}

DirectX::XMFLOAT3 SolidLightenedBall::GetPosition() const noexcept
{
    return world_position;
}

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
