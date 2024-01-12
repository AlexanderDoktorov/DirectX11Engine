#include "Buffer.h"
#include <ranges>
#include <iostream>
#include <Windows.h>
#include "Graphics.h"

namespace buffer
{
	namespace ranges = std::ranges;

	// ↓ bufferException 
	bufferException::bufferException(int line, std::string_view fileName, std::string_view msg)
		: baseException(line, fileName),
		msg(msg)
	{}
	std::string bufferException::what() const noexcept
	{
		return baseException::what() + msg;
	}
	 
	// ↓ BufferLayout
	Field BufferLayout::GetField(std::string_view semantic)
	{
		auto it = ranges::find_if(fields, [&semantic](Field& field) {
			return field.semantic == semantic;
			});
		if (it != fields.end())
			return *it;
		using namespace std::string_literals;
		OutputDebugStringA( ("Field not found by \""s + semantic.data() + "\" semantic\n").c_str());
		return Field();
	}

	// ↓ FieldProxy
	FieldProxy::FieldProxy(std::string_view semantic)
		:
		semantic(semantic)
	{}

	FieldProxy Buffer::operator[](std::string_view semantic)

	{
		const Field& field = layout.GetField(semantic);
		switch (field.fieldType)
		{
		case buffer::TYPE_FLOAT:
			return FieldProxy(reinterpret_cast<float*>(&bytes.at(field.byteOffset)), semantic);
		case buffer::TYPE_FLOAT3:
			return FieldProxy(reinterpret_cast<DirectX::XMFLOAT3*>(&bytes.at(field.byteOffset)), semantic);
		case buffer::TYPE_FLOAT4:
			return FieldProxy(reinterpret_cast<DirectX::XMFLOAT4*>(&bytes.at(field.byteOffset)), semantic);
		case buffer::TYPE_INT:
			return FieldProxy(reinterpret_cast<int*>(&bytes.at(field.byteOffset)), semantic);
		case buffer::TYPE_BOOL:
			return FieldProxy(reinterpret_cast<bool*>(&bytes.at(field.byteOffset)), semantic);
		}
		return FieldProxy(semantic);
	}

	const FieldProxy Buffer::operator[](std::string_view semantic) const
	{
		return const_cast<Buffer&>(*this)[semantic];
	}

	bool FieldProxy::Valid() const noexcept
	{
		return data != nullptr && dataType != TYPE_UNDEFINED;
	}

	// ↓ Buffer
	Buffer::Buffer(BufferLayout layout)
		:
		layout(layout)
	{
		bytes.resize(layout.byteSize);
	}

	const std::vector<std::byte>& Buffer::GetBytes() const noexcept
	{
		return bytes;
	}

	const size_t& Buffer::GetByteSize() const noexcept
	{
		return bytes.size();
	}

	// ↓ PixelConstantBufferEx
	PixelConstantBufferEx::PixelConstantBufferEx(Graphics& Gfx, const Buffer& buff, UINT bindSlot)
		:
		Slotted(bindSlot)
	{
		D3D11_BUFFER_DESC CBDesc = {};
		CBDesc.ByteWidth = static_cast<UINT>(buff.GetByteSize());
		CBDesc.Usage = D3D11_USAGE_DYNAMIC;
		CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA CBSubData = {};
		CBSubData.pSysMem = buff.GetBytes().data();

		HRESULT hr = GetDevice(Gfx)->CreateBuffer(&CBDesc, &CBSubData, &pBuffer); assert(SUCCEEDED(hr));
	}
	void PixelConstantBufferEx::Bind(Graphics& Gfx) noexcept
	{
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, pBuffer.GetAddressOf());
	}
}


