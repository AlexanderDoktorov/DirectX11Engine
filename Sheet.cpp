#include "Sheet.h"

DirectX::XMFLOAT4 Sheet::GetColor() const noexcept
{
    return color;
}

void Sheet::SetColor(dx::XMFLOAT4 new_color) noexcept
{
    color = new_color;
}
