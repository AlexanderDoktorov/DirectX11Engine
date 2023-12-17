#pragma once
#include <DirectXMath.h>
#include "assimp\material.h"

inline DirectX::XMFLOAT3 aiToDx(const aiColor3D& aiVec3)
{
	return *reinterpret_cast<const DirectX::XMFLOAT3*>(&aiVec3);
}

inline DirectX::XMFLOAT3 aiToDx(const aiVector3D& aiVec3)
{
	return *reinterpret_cast<const DirectX::XMFLOAT3*>(&aiVec3);
}