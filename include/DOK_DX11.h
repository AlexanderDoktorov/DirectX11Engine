#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

DirectX::XMMATRIX DOK_XMMatrixTranslation(const DirectX::XMFLOAT3& translations);
constexpr DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 lhs, DirectX::XMFLOAT3 rhs) noexcept
{
	return DirectX::XMFLOAT3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

bool operator==(const D3D11_BLEND_DESC& lhs, const D3D11_BLEND_DESC& rhs) noexcept;
bool operator!=(const D3D11_BLEND_DESC& lhs, const D3D11_BLEND_DESC& rhs) noexcept;