#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include "noxnd.h"

namespace dx = DirectX;

namespace CustomVertex
{
	enum VrtxAttribute
	{
		Position3D,
		Normal,
		Textcoord,
		Tangent
	};

	template<VrtxAttribute attribute>
	struct Map
	{

	};

	template<>
	struct Map<VrtxAttribute::Position3D>
	{
		using type = dx::XMFLOAT3;
		static constexpr const char* label = "POSITION";
		static constexpr DXGI_FORMAT dxgiformat = DXGI_FORMAT_R32G32B32_FLOAT;
	};

	template<>
	struct Map<VrtxAttribute::Normal>
	{
		using type = dx::XMFLOAT3;
		static constexpr const char* label = "NOMRAL";
		static constexpr DXGI_FORMAT dxgiformat = DXGI_FORMAT_R32G32B32_FLOAT;
	};

	template<>
	struct Map<VrtxAttribute::Textcoord>
	{
		using type = dx::XMFLOAT2;
		static constexpr const char* label = "TEXTCOORD";
		static constexpr DXGI_FORMAT dxgiformat = DXGI_FORMAT_R32G32_FLOAT;
	};

	template<>
	struct Map<VrtxAttribute::Tangent>
	{
		using type = dx::XMFLOAT3;
		static constexpr const char* label = "Tangent";
	};

	class VertexLayout
	{
	public:
		// VertexLayout element
		class VertexElement
		{
		public:
			VertexElement() = default;

			VertexElement(VrtxAttribute attribute) : attribute(attribute)
			{
			}

			DXGI_FORMAT GetDXGIFormat() const
			{
				switch (attribute)
				{
				case Position3D:
					return Map<Position3D>::dxgiformat;
					break;
				case Normal:
					return Map<Normal>::dxgiformat;
					break;
				case Textcoord:
					return Map<Textcoord>::dxgiformat;
					break;
				default:
					return DXGI_FORMAT_UNKNOWN;
				}
			}

			static const char* GetLabel(const VrtxAttribute& attribute) noexcept
			{
				switch (attribute)
				{
				case Position3D:
					return Map<Position3D>::label;
					break;
				case Normal:
					return Map<Normal>::label;
					break;
				case Textcoord:
					return Map<Textcoord>::label;
					break;
				default:
					return "Unknown label";
				}
			}

			static UINT GetByteSize(const VrtxAttribute& attribute) noexcept
			{
				switch (attribute)
				{
				case Position3D:
					return sizeof(Map<Position3D>::type);
					break;
				case Normal:
					return sizeof(Map<Normal>::type);
					break;
				case Textcoord:
					return sizeof(Map<Textcoord>::type);
					break;
				default:
					return 0U;
				}
			}

			VrtxAttribute GetAttribute() const
			{
				return attribute;
			}

			UINT GetByteSize() const noexcept
			{
				return GetByteSize(attribute);
			}

			const char* GetLabel() const noexcept
			{
				return GetLabel(attribute);
			}

			friend bool operator!=(const VertexElement& lhs, const VertexElement& rhs)
			{
				return lhs.attribute != rhs.attribute;
			}

		private:
			VrtxAttribute attribute = VrtxAttribute();
		};

	public:
		// Constructor
		VertexLayout() = default;

		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcept
		{
			UINT offset = 0U;
			std::vector<D3D11_INPUT_ELEMENT_DESC> result;
			for (size_t i = 0; i < elements.size(); i++)
			{
				D3D11_INPUT_ELEMENT_DESC desc{};
				desc.SemanticName = elements[i].GetLabel();
				desc.SemanticIndex = 0U;
				desc.Format = elements[i].GetDXGIFormat();
				desc.InputSlot = 0U;
				desc.AlignedByteOffset = offset; offset += elements[i].GetByteSize();
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0U;
				result.push_back(desc);
			}
			return result;
		}

		VertexLayout& Append(VrtxAttribute attr)
		{
			elements.push_back(std::move(VertexElement(attr)));
			return *this;
		}

		std::vector<VertexElement> GetElements() const noexcept
		{
			return elements;
		}

		size_t GetTotalByteSize() const noexcept
		{
			size_t total_size = 0U;
			for (size_t i = 0; i < elements.size(); i++)
			{
				total_size += elements[i].GetByteSize();
			}
			return total_size;
		}

		bool IsEmpty() const noexcept
		{
			return elements.empty();
		}

		friend bool operator==(const VertexLayout& lhs, const VertexLayout& rhs) noexcept
		{
			if (lhs.GetTotalByteSize() != rhs.GetTotalByteSize() || lhs.elements.size() != rhs.elements.size())
				return false;

			for (size_t i = 0; i < lhs.elements.size(); i++)
			{
				if (rhs.elements[i] != lhs.elements[i])
					return false;
			}
			return true;
		}

	private:
		std::vector<VertexElement> elements;
	};

	class Vertex
	{
		friend class VertexBuffer;
	public:
		Vertex(const VertexLayout& layout) : layout(layout)
		{
			data = new BYTE[layout.GetTotalByteSize()](0);
			data_size = layout.GetTotalByteSize();
		}

		Vertex(const VertexLayout& layout_, BYTE* pData) : Vertex(layout_)
		{
			memcpy(data, pData, data_size);
		}

		template<VrtxAttribute attr>
		Map<attr>::type& GetAttribute() const noxnd
		{
			auto offset = GetOffset(attr);
			using DXType = Map<attr>::type;
			return *(DXType*)(data + offset);
		}

		template<VrtxAttribute attr> 
		void SetAttribute(Map<attr>::type new_data) const noxnd
		{
			auto offset = GetOffset(attr);
			using DXType = Map<attr>::type;
			*(DXType*)(data + offset) = new_data;
		}

#ifdef _DEBUG
		template<typename T>
		void PrintDataAs() const noexcept
		{
			assert(data_size % sizeof(T) == 0);
			for (size_t i = 0; i < data_size; i += sizeof(T))
			{
				std::cout << *(T*)(&data[i]) << " data[" << i << "] " << std::endl;
			}
		}
#endif // _DEBUG

		~Vertex()
		{
			if (data) delete[] data;
		}

	private:

		UINT GetOffset(VrtxAttribute attr) const noxnd
		{
			auto layout_elements = layout.GetElements();
			UINT offset = 0U;
			for (size_t i = 0; i < layout_elements.size(); i++)
			{
				if (layout_elements[i].GetAttribute() == attr)
				{
					break;
				}
				else
				{
					offset += VertexLayout::VertexElement::GetByteSize(layout_elements[i].GetAttribute());
				}
			}
			assert(offset != data_size && "Unable to find vertex attribute");
			return offset;
		}


		size_t data_size = 0U;
		BYTE* data = nullptr;
		VertexLayout layout;
	};

	class VertexBuffer
	{
	public:

		VertexBuffer(const VertexLayout& layout) : layout(layout)
		{}

		void EmplaceBack(const Vertex& vrtx)
		{
			assert(layout == vrtx.layout && "Different layouts for vertex and buffer");
			verts.emplace_back(vrtx);
		}

		VertexLayout GetLayout() const noexcept
		{
			return layout;
		}

		UINT GetByteSize() const noexcept
		{
			return layout.GetTotalByteSize() * verts.size();
		}


		Vertex& operator[](unsigned int indx)
		{
			return verts[indx];
		}

	private:
		std::vector<Vertex> verts;
		VertexLayout layout;
	};


}

//namespace VertexMetaProgramming
//{
//	template<VrtxAttribute attribute0>
//	struct Vertex<attribute0>
//	{
//	public:
//		static constexpr VrtxAttribute vertexAttribute = attribute0;
//		using sys_type = Map<attribute0>::type;
//
//		template<VrtxAttribute attr>
//		sys_type& GetAttribute()
//		{
//			assert(attr == attribute0 && "Unable to find attribute");
//			return attrHolder;
//		}
//	private:
//		sys_type attrHolder = sys_type();
//	};
//
//	template<VrtxAttribute attribute0, VrtxAttribute... attributes>
//	struct Vertex
//	{
//		template<VrtxAttribute attr>
//		auto& GetAttribute()
//		{
//			if constexpr (attribute0 == attr)
//			{
//				return attrHolder;
//			}
//			else
//			{
//				return nextAttributeHolder.GetAttribute<attr>();
//			}
//		}
//
//		static constexpr std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DInputLayout()
//		{
//			return ;
//		}
//
//	private:
//
//		D3D11_INPUT_ELEMENT_DESC GenerateDesc() const noexcept
//		{
//			return { Map<attribute0>::label, 0, Map<attribute0>::dxgiformat, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
//		}
//
//		using sys_type = Map<attribute0>::type;
//		sys_type attrHolder = sys_type();
//		static constexpr VrtxAttribute vertexAttribute = attribute0;
//		Vertex<attributes...> nextAttributeHolder;
//	};
//}