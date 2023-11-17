#pragma once
#include <DirectXMath.h>

DirectX::XMMATRIX DOK_XMMatrixTranslation(const DirectX::XMFLOAT3& translations);
DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 lhs, DirectX::XMFLOAT3 rhs) noexcept;
