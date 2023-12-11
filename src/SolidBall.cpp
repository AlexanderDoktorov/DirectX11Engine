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


void SolidBall::SetScaleX(const float& scale_x_new)
{
    scale.x = scale_x_new;
}

void SolidBall::SetScaleY(const float& scale_y_new)
{
    scale.y = scale_y_new;
}

void SolidBall::SetScaleZ(const float& scale_z_new)
{
    scale.z = scale_z_new;
}

dx::XMFLOAT3 SolidBall::GetScale() const noexcept
{
    return scale;
}

