#include "../include/SolidTexturedBall.h"

void SolidTexturedBall::SetPosition(float _x, float _y, float _z)
{
    world_position.x = _x;
    world_position.y = _y;
    world_position.z = _z;
}

DirectX::XMFLOAT3 SolidTexturedBall::GetPosition() const noexcept
{
    return world_position;
}

const char* SolidTexturedBall::ToString() const noexcept
{
    return "Solid Textured Ball";
}