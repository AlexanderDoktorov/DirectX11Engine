#include "DOK_DX11.h"

DirectX::XMMATRIX DOK_XMMatrixTranslation(const DirectX::XMFLOAT3& translations)
{
	return DirectX::XMMatrixTranslation(translations.x, translations.y, translations.z);
}

DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 lhs, DirectX::XMFLOAT3 rhs) noexcept
{
	return DirectX::XMFLOAT3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
