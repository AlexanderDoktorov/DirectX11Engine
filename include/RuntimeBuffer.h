#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <DirectXMath.h>
#include <stdexcept>
#include <typeinfo>
#include <iostream>
#include <wrl.h>
#include "noxnd.h"
#include "baseException.h"
#include "IBindable.h"
#include "ISlot.h"

namespace RuntimeBuffer
{
	namespace ranges = std::ranges;
	class FieldProxy;

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

	class BufferLayout
	{
		friend class Buffer;
	public:
		struct Field
		{
			TYPE   dataType		 = TYPE_UNDEFINED; 
			size_t byteOffset	 = 0ull;
			std::string semantic = "Unknown semantic";
		};
		using FieldIterator = std::vector<Field>::iterator;
		using FieldConstIterator = std::vector<Field>::const_iterator;
	public:
		BufferLayout() = default;
		template<Defined T>
		void AddField(std::string semantic) noxnd
		{
			auto it = std::find_if(fields.begin(), fields.end(), [&semantic](Field& f) {
				return f.semantic == semantic;
			});
			if (it == fields.end()) {
				constexpr const size_t fieldSize = map<reverse_map<T>::t>::size;
				if (Crosses16Border(fieldSize) && (byteSize % 16 != 0) )
					fields.emplace_back(reverse_map<T>::t, CalcBorderOffset(), std::move(semantic));
				else
					fields.emplace_back(reverse_map<T>::t, byteSize, std::move(semantic));
				byteSize = fields.back().byteOffset + fieldSize;
			}
			else
				assert(false && "Layout already has such field");
		}
		FieldConstIterator FindConstField(const std::string& semantic) const;
		FieldIterator      FindField(const std::string& semantic);
		FieldIterator      Begin() noexcept;
		FieldIterator      End() noexcept;
		BufferLayout&	   AlignAs16() noexcept;
		size_t GetByteSize() const noexcept;
	private:
		bool   Crosses16Border(size_t fieldSize) const noexcept;
		size_t CalcBorderOffset() const noexcept;
		std::vector<Field> fields;
		size_t byteSize = 0ull;
	};

	// ↓ BufferIterator
	class BufferIterator
	{
		friend Buffer;
		friend FieldProxy;
	public:
		BufferIterator(void* data, BufferLayout::FieldIterator itField);
		BufferIterator& operator++() noexcept;
		FieldProxy operator*();
	private:
		void* data	= nullptr;
		BufferLayout::FieldIterator itField;
	};

	// ↓ FieldProxy
	class FieldProxy
	{
		class Ptr
		{
		public:
			Ptr(FieldProxy* ptrFieldProxy) :
				ptrFieldProxy(ptrFieldProxy)
			{}
			// conversion for getting read-only pointer to supported SysType
			template<class T>
			operator const T*() const noxnd
			{
				return &static_cast<const T&>(*ptrFieldProxy);
			}
			template<class T>
			operator T*() noxnd
			{
				const T& valCRef = static_cast<const T&>(*ptrFieldProxy);
				return &const_cast<T&>(valCRef);
			}
		private:
			FieldProxy* ptrFieldProxy = nullptr;
		};
	public:
		FieldProxy(BufferIterator& itBuff);
		template<Defined T>
		operator const T&() const {
			using namespace std::string_literals;
			if (!data)
				throw bufferException(__LINE__, __FILE__, "Semantic \""s + field.semantic + "\" does not exist in buffer");
			if(!IsTypeConvertableTo<T>(field.dataType))
				throw bufferException(__LINE__, __FILE__, "Unable to convert "s + TypeStr(field.dataType) + " into " + typeid(T).name());
			return *(reinterpret_cast<T*>(data));
		}
		Ptr operator&()
		{
			return Ptr{ this };
		}
		
		template<class T>
		FieldProxy& operator=(T&& value)
		{
			using namespace std::string_literals;
			if (!data)
				throw bufferException(__LINE__, __FILE__, "Semantic \""s + field.semantic + "\" does not exist in buffer");
			switch (field.dataType)
			{
#define F(t) case t: assignValue<t>(data, std::forward<T>(value)); break;
				TYPES_APPLY_F(F)
#undef F
			}
			return *this;
		}
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
		void* data = nullptr;
		BufferLayout::Field field{};
	};

	class Buffer
	{
		friend class PixelConstantBufferEx;
	public:
		Buffer() = default;
		Buffer(BufferLayout layout);
		FieldProxy operator[](const std::string& semantic);
		const FieldProxy operator[](const std::string& semantic) const;
		const std::vector<std::byte>& GetBytes() const noexcept;
		const BufferLayout& GetLayout() const noexcept;
		size_t GetByteSize() const noexcept;
		BufferIterator Begin() noexcept;
		BufferIterator End() noexcept;
	private:
		BufferLayout layout;
		std::vector<std::byte> bytes;
	};

	class PixelConstantBufferEx : public IBindable, public Slotted
	{
	public:
		PixelConstantBufferEx() = default;
		PixelConstantBufferEx(Graphics& Gfx, const BufferLayout& layout, Buffer* ptrBuff, UINT bindSlot = 0U);
		void Bind(Graphics& Gfx) noexcept override;
		void Update(Graphics& Gfx, const Buffer* buffer);
		operator bool() const noexcept { return pBuffer != nullptr; }
	private:
		Microsoft::WRL::ComPtr<struct ID3D11Buffer> pBuffer;
	};
}