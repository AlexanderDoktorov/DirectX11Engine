#pragma once
#include "IBindable.h"
#include "ISlot.h"
#include "ITexture2D.h"
#include <wrl.h>
#include <wincodec.h>
#include "WICTextureLoader11.h"

class TextureBase : public IBindable, public Slotted, public ITexture2D
{
public:
	TextureBase();
	TextureBase(UINT bindSlot);

	virtual void Bind(Graphics& Gfx) noexcept override;
	virtual ID3D11Texture2D*				GetTexture() const noexcept override;
	virtual ID3D11ShaderResourceView*		GetSRV() const noexcept override;
	virtual D3D11_TEXTURE2D_DESC			GetDesc() const noexcept override;	

protected:

	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};