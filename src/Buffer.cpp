﻿#include "Buffer.h"
#include <iostream>
#include <Windows.h>
#include "Graphics.h"

namespace buffer
{
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
	BufferLayout::FieldConstIterator BufferLayout::FindConstField(const std::string& semantic) const
	{
		return const_cast<BufferLayout&>(*this).FindField(semantic);
	}

	BufferLayout::FieldIterator BufferLayout::FindField(const std::string& semantic)
	{
		FieldIterator it = std::find_if(fields.begin(), fields.end(), [&semantic](auto& f) {
			return f.semantic == semantic;
		});
		if (it == fields.end())
		{
			using namespace std::string_literals;
			OutputDebugStringA( ("Field not found by \""s + semantic.data() + "\" semantic\n").c_str());
		}
		return it;
	}

	BufferLayout::FieldIterator BufferLayout::Begin() noexcept
	{
		return fields.begin();
	}

	BufferLayout::FieldIterator BufferLayout::End() noexcept
	{
		return fields.end();
	}

	size_t BufferLayout::GetByteSize() const noexcept
	{
		return byteSize;
	}

	bool BufferLayout::Crosses16Border(size_t fieldSize) const noexcept
	{
		const auto end = byteSize + fieldSize;
		const auto pageStart = byteSize / 16;
		const auto pageEnd   = end / 16;
		return (pageStart != pageEnd && end % 16 != 0u) || fieldSize > 16;
	}

	size_t BufferLayout::CalcBorderOffset() const noexcept
	{
		return byteSize + (16ull - byteSize % 16ull);
	}

	// ↓ BufferIterator
	BufferIterator::BufferIterator(void* data, BufferLayout::FieldIterator itField)
		:
		data(data),
		itField(itField)
	{
	}

	BufferIterator& BufferIterator::operator++() noexcept
	{
		switch (itField->dataType)
		{
#define F(t) case t: data = reinterpret_cast<void*>(reinterpret_cast<map<t>::type*>(data) + 1); ++itField; break;
			TYPES_APPLY_F(F)
#undef F
		}
		return *this;
	}

	// ↓ FieldProxy
	FieldProxy::FieldProxy(BufferIterator& itBuff) 
		:
		data(itBuff.data),
		field(*itBuff.itField)
	{

	}

	FieldProxy BufferIterator::operator*()
	{
		return FieldProxy(*this);
	}

	// ↓ Buffer
	Buffer::Buffer(BufferLayout layout)
		:
		layout(std::move(layout)),
		bytes(this->layout.byteSize)
	{
	}

	FieldProxy Buffer::operator[](const std::string& semantic)
	{
		auto itField = layout.FindField(semantic);
		switch (itField->dataType)
		{
		case buffer::TYPE_FLOAT:
			return *BufferIterator(&bytes.at(itField->byteOffset), itField);
		case buffer::TYPE_FLOAT3:
			return *BufferIterator(&bytes.at(itField->byteOffset), itField);
		case buffer::TYPE_FLOAT4:
			return *BufferIterator(&bytes.at(itField->byteOffset), itField);
		case buffer::TYPE_INT:
			return *BufferIterator(&bytes.at(itField->byteOffset), itField);
		case buffer::TYPE_BOOL:
			return *BufferIterator(&bytes.at(itField->byteOffset), itField);
		}
		return *BufferIterator(nullptr, itField);
	}

	const FieldProxy Buffer::operator[](const std::string& semantic) const
	{
		return const_cast<Buffer&>(*this)[semantic];
	}

	BufferIterator Buffer::Begin() noexcept
	{
		return BufferIterator(bytes.data(), layout.Begin());
	}

	BufferIterator Buffer::End() noexcept
	{
		return BufferIterator(nullptr, layout.End());
	}

	const std::vector<std::byte>& Buffer::GetBytes() const noexcept
	{
		return bytes;
	}

	size_t Buffer::GetByteSize() const noexcept
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
	// ↓ CachingPixelConstantBufferEx
	CachingPixelConstantBufferEx::CachingPixelConstantBufferEx(Graphics& Gfx, Buffer& buff, UINT bindSlot)
		:
		buffRef(buff)
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
	void CachingPixelConstantBufferEx::Bind(Graphics& Gfx) noexcept
	{
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, pBuffer.GetAddressOf());
	}
	void CachingPixelConstantBufferEx::Update(Graphics& Gfx) noexcept
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
		HRESULT hr = GetContext(Gfx)->Map(pBuffer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &mappedSubresource); assert(SUCCEEDED(hr));

		memcpy(mappedSubresource.pData, buffRef.GetBytes().data(), buffRef.GetByteSize());

		GetContext(Gfx)->Unmap(pBuffer.Get(), 0U);
	}
}


