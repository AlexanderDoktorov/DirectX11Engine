#pragma once
#include "IBindable.h"

template<class T>
class ResourseBuffer : public IBindable
{
public:
	ResourseBuffer(Graphics& Gfx, const T& bufferData)
	{
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = sizeof(bufferData);
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA bufferSrd{};
		bufferSrd.pSysMem = &bufferData;

		CHECK_HR( GetDevice(Gfx)->CreateBuffer(&bufferDesc, &bufferSrd, &pBuffer) );
	}

	ResourseBuffer(Graphics& Gfx)
	{
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		CHECK_HR( GetDevice(Gfx)->CreateBuffer(&bufferDesc, &nullptr, &pBuffer) );
	}

	void Update(Graphics& Gfx, const T& NewData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource{};
		CHECK_HR( GetContext(Gfx)->Map(pBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedSubresource) );

		memcpy(mappedSubresource.pData, &NewData, sizeof(NewData));

		GetContext(Gfx)->Unmap(pBuffer.Get(), 0U);
	}
	
	~ResourseBuffer() = default;

protected:
	wrl::ComPtr<ID3D11Buffer> pBuffer;
};