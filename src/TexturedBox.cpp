#include "TexturedBox.h"

DirectX::XMMATRIX TexturedBox::GetTransform() const noexcept
{
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixTranslation(x, y, z);
}
void TexturedBox::SetPosition(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

DirectX::XMFLOAT3 TexturedBox::GetPosition() const noexcept
{
	return dx::XMFLOAT3(x,y,z);
}

void TexturedBox::SetScaleX(const float& scale_x_new)
{
	scale.x = scale_x_new;
}

void TexturedBox::SetScaleY(const float& scale_y_new)
{
	scale.y = scale_y_new;
}

void TexturedBox::SetScaleZ(const float& scale_z_new)
{
	scale.z = scale_z_new;
}

dx::XMFLOAT3 TexturedBox::GetScale() const noexcept
{
	return scale;
}

const char* TexturedBox::ToString() const noexcept
{
	return "Textured box";
}
