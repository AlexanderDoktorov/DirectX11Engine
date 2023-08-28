#include "DOK_DX11.h"

DirectX::XMMATRIX DOK_XMMatrixTranslation(const DirectX::XMFLOAT3& translations)
{
	return DirectX::XMMatrixTranslation(translations.x, translations.y, translations.z);
}
