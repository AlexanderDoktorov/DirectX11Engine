#include "Buffer.h"

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
	Field BufferLayout::GetField(std::string_view semantic)
	{
		auto it = ranges::find_if(fields, [&semantic](Field& field) {
			return field.semantic == semantic;
			});
		if (it != fields.end())
			return *it;
		using namespace std::string_literals;
		OutputDebugStringA( ("Field not found by [ "s + semantic.data() + " ] semantic\n").c_str());
		return Field();
	}

	// ↓ FieldProxy
	bool FieldProxy::Valid() const noexcept
	{
		return data != nullptr && dataType != TYPE_UNDEFINED;
	}

	// ↓ Buffer
	Buffer::Buffer(BufferLayout layout)
		:
		layout(layout)
	{
		buff.resize(layout.byteSize);
	}

	FieldProxy Buffer::operator[](std::string_view semantic)

	{
		const Field& field = layout.GetField(semantic);
		switch (field.fieldType)
		{
		case buffer::TYPE_FLOAT:
			return FieldProxy(reinterpret_cast<float*>(&buff.at(field.byteOffset)));
		case buffer::TYPE_FLOAT3:
			return FieldProxy(reinterpret_cast<DirectX::XMFLOAT3*>(&buff.at(field.byteOffset)));
		case buffer::TYPE_FLOAT4:
			return FieldProxy(reinterpret_cast<DirectX::XMFLOAT4*>(&buff.at(field.byteOffset)));
		case buffer::TYPE_INT:
			return FieldProxy(reinterpret_cast<int*>(&buff.at(field.byteOffset)));
		case buffer::TYPE_BOOL:
			return FieldProxy(reinterpret_cast<bool*>(&buff.at(field.byteOffset)));
		}
		return FieldProxy();
	}

	const FieldProxy Buffer::operator[](std::string_view semantic) const
	{
		return const_cast<Buffer&>(*this)[semantic];
	}

}
