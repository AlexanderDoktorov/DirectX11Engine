#include "DOK_DX11.h"

DirectX::XMMATRIX DOK_XMMatrixTranslation(const DirectX::XMFLOAT3& translations)
{
	return DirectX::XMMatrixTranslation(translations.x, translations.y, translations.z);
}

bool operator==(const D3D11_BLEND_DESC& lhs, const D3D11_BLEND_DESC& rhs) noexcept
{
	bool equal = true;
	for (size_t i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		equal &= (lhs.RenderTarget[i].BlendEnable == rhs.RenderTarget[i].BlendEnable);
		equal &= (lhs.RenderTarget[i].BlendOp == rhs.RenderTarget[i].BlendOp);
		equal &= (lhs.RenderTarget[i].BlendOpAlpha == rhs.RenderTarget[i].BlendOpAlpha);
		equal &= (lhs.RenderTarget[i].DestBlend == rhs.RenderTarget[i].DestBlend);
		equal &= (lhs.RenderTarget[i].DestBlendAlpha == rhs.RenderTarget[i].DestBlendAlpha);
		equal &= (lhs.RenderTarget[i].RenderTargetWriteMask == rhs.RenderTarget[i].RenderTargetWriteMask);
		equal &= (lhs.RenderTarget[i].SrcBlend == rhs.RenderTarget[i].SrcBlend);
		equal &= (lhs.RenderTarget[i].SrcBlendAlpha == rhs.RenderTarget[i].SrcBlendAlpha);
		if (!equal)
			return false;
		if (lhs.IndependentBlendEnable == FALSE && rhs.IndependentBlendEnable == FALSE)
			break;
	}
	equal &=  ( lhs.AlphaToCoverageEnable == rhs.AlphaToCoverageEnable );
	equal &=  ( lhs.IndependentBlendEnable == rhs.IndependentBlendEnable );

	return equal;
}

bool operator!=(const D3D11_BLEND_DESC& lhs, const D3D11_BLEND_DESC& rhs) noexcept
{
	return !operator==(lhs, rhs);
}
