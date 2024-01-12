#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <stdexcept>
#include <typeinfo>
#include "baseException.h"
#include <iostream>
#include <wrl.h>
#include "IBindable.h"
#include "ISlot.h"

namespace buffer
{
#define TYPES_APPLY_F(F) \
	F(TYPE_FLOAT) \
	F(TYPE_FLOAT3) \
	F(TYPE_FLOAT4) \
	F(TYPE_INT) \
	F(TYPE_BOOL)

	enum TYPE
	{
		TYPE_UNDEFINED,
		TYPE_FLOAT,
		TYPE_FLOAT3,
		TYPE_FLOAT4,
		TYPE_INT,
		TYPE_BOOL
	};

	struct defined {
		static constexpr bool is_defined = true;
	};

	struct undefined {
		static constexpr bool is_defined = false;
	};

	template<class T, size_t S>
	struct map_def : defined
	{
		using type = T;
		static constexpr size_t size = S;
	};

	template<TYPE t>
	struct reverse_map_def : defined
	{
		static constexpr TYPE t = t;
	};

	template<enum TYPE> struct map		: public undefined {};
	template<> struct map<TYPE_FLOAT>	: public map_def<float, sizeof(float)> {};
	template<> struct map<TYPE_FLOAT3>  : public map_def<DirectX::XMFLOAT3, sizeof(DirectX::XMFLOAT3)> {};
	template<> struct map<TYPE_FLOAT4>	: public map_def<DirectX::XMFLOAT4, sizeof(DirectX::XMFLOAT4)> {};
	template<> struct map<TYPE_INT>		: public map_def<int, sizeof(int)> {};
	template<> struct map<TYPE_BOOL>	: public map_def<bool, 4ull> {};

	template<class T> struct reverse_map : public undefined {};
	template<> struct reverse_map<float>				: public reverse_map_def<TYPE_FLOAT>	{};
	template<> struct reverse_map<DirectX::XMFLOAT3>	: public reverse_map_def<TYPE_FLOAT3>	{};
	template<> struct reverse_map<DirectX::XMFLOAT4>	: public reverse_map_def<TYPE_FLOAT4>	{};
	template<> struct reverse_map<int>					: public reverse_map_def<TYPE_INT>		{};
	template<> struct reverse_map<bool>					: public reverse_map_def<TYPE_BOOL>	{};

	template<class T>
	concept Defined = reverse_map<T>::is_defined && map<reverse_map<T>::t>::is_defined;

	template <typename Tgt, typename Src>
	concept NoNarrowConvertive = requires (Src s) { Tgt{ s }; };

	static constexpr const char* TypeStr(TYPE t) noexcept
	{
		switch (t)
		{
#define NAME_F(type) case type: return #type;
			TYPES_APPLY_F(NAME_F)
#undef NAME_F
		}
		return "UNKNOWN_TYPE";
	}

	template<class T>
	bool IsTypeAssignableTo(const TYPE& t) noexcept
	{
		switch (t)
		{
#define F(t) case t: return std::is_assignable_v<typename map<t>::type&,T>;
			TYPES_APPLY_F(F)
#undef F
		}
		return false;
	}

	template<class T>
	bool IsTypeConvertableTo(const TYPE& t) noexcept
	{
		switch (t)
		{
#define F(t) case t: return std::is_convertible_v<typename map<t>::type,T>;
			TYPES_APPLY_F(F)
#undef F
		}
		return false;
	}

	class bufferException : public baseException
	{
	public:
		bufferException(int line, std::string_view fileName, std::string_view msg);
		std::string what() const noexcept override;
	private:
		std::string msg;
	};

	struct Field
	{
		std::string semantic = "Unknown semantic";
		TYPE fieldType		 = TYPE_UNDEFINED; 
		size_t byteOffset    = 0;
	};

	class BufferLayout
	{
		friend class Buffer;
	public:
		template<Defined T> 
		void AddField(std::string_view semantic)
		{
			Field field;
			field.byteOffset = byteSize;
			field.semantic	 = semantic;
			field.fieldType  = reverse_map<T>::t;

			fields.push_back(field);
			byteSize += map<reverse_map<T>::t>::size;
		}
		Field GetField(std::string_view semantic);
	private:
		std::vector<Field> fields;
		size_t byteSize = 0U;
	};

	class FieldProxy
	{
		friend class Buffer;
	public:
		FieldProxy() = default;
		FieldProxy(std::string_view semantic);
		template<Defined T>
		FieldProxy(T* data, std::string_view semantic) 
			:
			data(data), 
			dataType(reverse_map<T>::t),
			semantic(semantic)
		{}
		template<class T>
		operator T&() {
			using namespace std::string_literals;
			if (!data)
				throw bufferException(__LINE__, __FILE__, "Semantic \""s + semantic.data() + "\" does not exist in buffer");
			if(!IsTypeConvertableTo<T>(dataType))
				throw bufferException(__LINE__, __FILE__, "Unable to convert "s + TypeStr(dataType) + " into " + typeid(T).name());
			return *(reinterpret_cast<T*>(data));
		}
		template<Defined T>
		operator const T&() const {
			return static_cast<T&>(const_cast<FieldProxy&>(*this));
		}
		template<class T>
		FieldProxy& operator=(T&& value)
		{
			using namespace std::string_literals;
			if (!data)
				throw bufferException(__LINE__, __FILE__, "Semantic \""s + semantic.data() + "\" does not exist in buffer");
			switch (dataType)
			{
#define F(t) case t: assignValue<t>(data, std::forward<T>(value)); break;
			TYPES_APPLY_F(F)
#undef F
			}
			return *this;
		}
		bool Valid() const noexcept;
	protected:
		template <TYPE t, class T> 
			requires std::is_assignable_v<typename map<t>::type&, T> && NoNarrowConvertive<typename map<t>::type, T>
		void assignValue(void* data, T&& value)
		{
			*reinterpret_cast<typename map<t>::type*>(data) = std::forward<T>(value);
		}
		template <TYPE t, class T>
		void assignValue(void* data, T&& value)
		{
			using namespace std::string_literals;
			throw bufferException(__LINE__, __FILE__, "Unable to assign "s + TypeStr(t) + " to a " + typeid(T).name());
		}
	private:
		void* data			 = nullptr;
		TYPE  dataType		 = TYPE_UNDEFINED;
		std::string_view semantic = "Unknown semantic";
	};

	class Buffer
	{
	public:
		Buffer(BufferLayout layout);
		FieldProxy operator[](std::string_view semantic);
		const FieldProxy operator[](std::string_view semantic) const;
		const std::vector<std::byte>& GetBytes() const noexcept;
		const size_t& GetByteSize() const noexcept;
	private:
		BufferLayout layout;
		std::vector<std::byte> bytes;
	};

	class PixelConstantBufferEx : public IBindable, public Slotted
	{
	public:
		PixelConstantBufferEx(Graphics& Gfx, const Buffer& buff, UINT bindSlot = 0U);
		void Bind(Graphics& Gfx) noexcept override;
	private:
		Microsoft::WRL::ComPtr<struct ID3D11Buffer> pBuffer;
	};
}