#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <DirectXMath.h>
#include <ranges>
#include <Windows.h>
#include <stdexcept>
#include <typeinfo>
#include "baseException.h"
#include <iostream>

namespace buffer
{
	namespace ranges = std::ranges;
#define TYPES_APPLY_F(F) \
	F(TYPE_UNDEFINED) \
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

	static constexpr const char* TypeStr(TYPE t) noexcept
	{
		switch (t)
		{
#define GENERATE_SWITCH(type) case type: return #type;
		TYPES_APPLY_F(GENERATE_SWITCH)
		}
		return "UNKNOWN_TYPE";
	}

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
		std::string semantic = "Unknown";
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
		friend Buffer;
	public:
		FieldProxy() = default;
		template<Defined T>
		FieldProxy(T* data) 
			:
			data(data), 
			dataType(reverse_map<T>::t)
		{}
		template<Defined T>
		operator T&() {
			using namespace std::string_literals;
			if (!data)
				throw bufferException(__LINE__, __FILE__, "Trying to get non-existent value from buffer");
			if(reverse_map<T>::t != dataType)
				std::cerr << bufferException(__LINE__, __FILE__, "Unable to convert "s + TypeStr(dataType) + " into " + typeid(T).name()).what();
			return *(reinterpret_cast<T*>(data));
		}

		template<Defined T>
		operator const T&() const {
			return (T&)const_cast<FieldProxy&>(*this);
		}
		bool Valid() const noexcept;
	private:
		void* data	  = nullptr;
		TYPE dataType = TYPE_UNDEFINED;
	};

	class Buffer
	{
	public:
		Buffer(BufferLayout layout);
		FieldProxy operator[](std::string_view semantic);
		const FieldProxy operator[](std::string_view semantic) const;
	private:
		BufferLayout layout;
		std::vector<std::byte> buff;
	};

	
}