#pragma once
#include <vector>
#include <type_traits>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

namespace DynamicVertex
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Tangent,
			Bitangent,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count,
		};
		template<ElementType> struct Map;
		template<> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const char* code = "P2";
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const char* code = "P3";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
			static constexpr const char* code = "T2";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
			static constexpr const char* code = "N";
		};
		template<> struct Map<Tangent>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Tangent";
			static constexpr const char* code = "Nt";
		};
		template<> struct Map<Bitangent>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Bitangent";
			static constexpr const char* code = "Nb";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C3";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C4";
		};

		class Element
		{
		public:
			Element( ElementType type,size_t offset );
			size_t GetOffsetAfter() const;
			size_t GetOffset() const;
			size_t Size() const;
			static constexpr size_t SizeOf( ElementType type );
			ElementType GetType() const noexcept;
			D3D11_INPUT_ELEMENT_DESC GetDesc() const;
			const char* GetCode() const noexcept;
		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc( size_t offset ) noexcept
			{
				return { Map<type>::semantic,0,Map<type>::dxgiFormat,0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
			}
		private:
			ElementType type;
			size_t offset;
		};
	public:

		VertexLayout() = default;

		VertexLayout(std::vector<ElementType> elements)
		{
			for (auto el : elements)
			{
				Append(el);
			}
		}

		template<ElementType Type>
		const Element& Resolve() const
		{
			for( auto& e : elements )
			{
				if( e.GetType() == Type )
				{
					return e;
				}
			}
			assert( "Could not resolve element type" && false );
			return elements.front();
		}

		template<ElementType Type>
		bool HasSuchElement() const
		{
			for( auto& e : elements )
			{
				if( e.GetType() == Type )
				{
					return true;
				}
			}
			return false;
		}

		const Element& ResolveByIndex( size_t i ) const;
		VertexLayout& Append( ElementType type );
		size_t Size() const;
		size_t GetElementCount() const noexcept;
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const;
		std::string GetCode() const;
	private:
		std::vector<Element> elements;
	};

	class Vertex
	{
		friend class VertexBuffer;
	public:
		template<VertexLayout::ElementType Type>
		auto& Attr()
		{
			auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}
		template<typename T>
		void SetAttributeByIndex( size_t i,T&& val )
		{
			const auto& element = layout.ResolveByIndex( i );
			auto pAttribute = pData + element.GetOffset();
			switch( element.GetType() )
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>( pAttribute,std::forward<T>( val ) );
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>( pAttribute,std::forward<T>( val ) );
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>( pAttribute,std::forward<T>( val ) );
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>( pAttribute,std::forward<T>( val ) );
				break;
			case VertexLayout::Tangent:
				SetAttribute<VertexLayout::Tangent>( pAttribute,std::forward<T>( val ) );
				break;
			case VertexLayout::Bitangent:
				SetAttribute<VertexLayout::Bitangent>( pAttribute,std::forward<T>( val ) );
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>( pAttribute,std::forward<T>( val ) );
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>( pAttribute,std::forward<T>( val ) );
				break;
			default:
				assert( "Bad element type" && false );
			}
		}
	protected:
		Vertex( char* pData,const VertexLayout& layout );
	private:
		// enables parameter pack setting of multiple parameters by element index
		template<typename First,typename ...Rest>
		void SetAttributeByIndex( size_t i,First&& first,Rest&&... rest )
		{
			SetAttributeByIndex( i,std::forward<First>( first ) );
			SetAttributeByIndex( i + 1,std::forward<Rest>( rest )... );
		}
		// helper to reduce code duplication in SetAttributeByIndex
		template<VertexLayout::ElementType DestLayoutType,typename SrcType>
		void SetAttribute( char* pAttribute,SrcType&& val )
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr( std::is_assignable<Dest,SrcType>::value )
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				assert( "Parameter attribute type mismatch" && false );
			}
		}
	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};

	class ConstVertex
	{
	public:
		ConstVertex( const Vertex& v );
		template<VertexLayout::ElementType Type>
		const auto& Attr() const
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}
	private:
		Vertex vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer( VertexLayout layout,size_t size = 0u );
		const char* GetData() const;
		const VertexLayout& GetLayout() const noexcept;
		void Resize( size_t newSize );
		size_t Size() const;
		size_t SizeBytes() const;
		template<typename ...Params>
		void EmplaceBack( Params&&... params )
		{
			assert( sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements" );
			buffer.resize( buffer.size() + layout.Size() );
			Back().SetAttributeByIndex( 0u,std::forward<Params>( params )... );
		}
		template<VertexLayout::ElementType el, typename T = VertexLayout::Map<el>::SysType>
		std::vector<T> GetAllThe() const noexcept
		{
			std::vector<T> result;
			result.reserve(Size());
			for (size_t i = 0; i < Size(); i++)
			{
				result.push_back((*this)[i].Attr<el>());
			}
			return result;
		}
		template<VertexLayout::ElementType el, typename T = VertexLayout::Map<el>::SysType>
		void SetAllThe(const std::vector<T>& data) noexcept
		{
			using namespace DirectX;
			assert ( data.size() <= Size() );
			for (size_t i = 0; i < data.size(); i++)
			{
				(*this)[i].Attr<el>() = data[i];
			}
		}
		Vertex Back();
		Vertex Front();
		Vertex operator[]( size_t i ) ;
		ConstVertex Back() const;
		ConstVertex Front() const;
		ConstVertex operator[]( size_t i ) const;
	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
}
