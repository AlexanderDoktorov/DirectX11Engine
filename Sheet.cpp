#include "Sheet.h"

void Sheet::SetPosition(float _x, float _y, float _z)
{
    x = _x; y = _y; z = _z;
}

DirectX::XMFLOAT3 Sheet::GetPosition() const noexcept
{
    return DirectX::XMFLOAT3(x,y,z);
}

DirectX::XMFLOAT4 Sheet::GetColor() const noexcept
{
    return color;
}

void Sheet::SetColor(dx::XMFLOAT4 new_color) noexcept
{
    color = new_color;
}

void Sheet::SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new)
{
    scale_x = scale_x_new;
    scale_y = scale_y_new;
    scale_z = scale_z_new;
}

dx::XMFLOAT3 Sheet::GetScale() const noexcept
{
    return dx::XMFLOAT3(scale_x,scale_y,scale_z);
}
