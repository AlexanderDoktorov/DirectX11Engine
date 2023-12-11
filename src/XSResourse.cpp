#include "XSResourse.h"
	
void PSResourse::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* pSRV = GetSRV();
	GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, &pSRV);
}

void VSResourse::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* pSRV = GetSRV();
	GetContext(Gfx)->VSSetShaderResources(GetBindSlot(), 1U, &pSRV);
}

void HSResourse::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* pSRV = GetSRV();
	GetContext(Gfx)->HSSetShaderResources(GetBindSlot(), 1U, &pSRV);
}

void CSResourse::Bind(Graphics& Gfx) noexcept
{
	ID3D11ShaderResourceView* pSRV = GetSRV();
	GetContext(Gfx)->CSSetShaderResources(GetBindSlot(), 1U, &pSRV);
}

ShaderResourse::ShaderResourse(UINT bindSlot)
	: Slotted(bindSlot)  {}