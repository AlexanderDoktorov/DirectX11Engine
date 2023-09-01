#pragma once
#include "ResourseBuffer.h"
#include "LightSource.h"
#include <array>

#define MAX_NUM_LIGTHS 20U

template <size_t buff_size = 20U>
class LightResourseBuffer : public ResourseBuffer<std::array<LightDesc, buff_size>>
{
	using IBindable::GetContext;
	using IBindable::GetDevice;
	using ResourseBuffer<std::array<LightDesc, buff_size>>::pBuffer;
public:

	LightResourseBuffer(Graphics& Gfx, const std::array<LightDesc, buff_size>& lightDescs) : ResourseBuffer<std::array<LightDesc, buff_size>>(Gfx, lightDescs)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//srvDesc.Format =; // Use appropriate format
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.ElementOffset = 0;
		srvDesc.Buffer.ElementWidth = lightDescs.size(); // Number of elements in the buffer

		CHECK_HR ( GetDevice(Gfx)->CreateShaderResourceView(pBuffer.Get(), &srvDesc, &pShaderResourceView) );
	}

	void Bind(Graphics& Gfx) noexcept override
	{
		GetContext(Gfx)->CSSetShaderResources(0U, 1U, pShaderResourceView.GetAddressOf());
	}

	void Unbind(Graphics& Gfx) noexcept
	{
		GetContext(Gfx)->CSSetShaderResources(0U, 1U, nullptr);
	}

private:
	wrl::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
};