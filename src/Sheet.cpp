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