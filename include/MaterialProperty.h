#pragma once
#include <DirectXMath.h>
#include "imgui.h"
#include "assimp\material.h"
#include <string>
#include <vector>

template<class T>
struct PMap
{};

template<> struct PMap<float>		{ static constexpr aiPropertyTypeInfo value = aiPTI_Float;	};
template<> struct PMap<aiColor3D>	{ static constexpr aiPropertyTypeInfo value = aiPTI_Float;	};
template<> struct PMap<int>			{ static constexpr aiPropertyTypeInfo value = aiPTI_Integer;};
template<> struct PMap<aiString>	{ static constexpr aiPropertyTypeInfo value = aiPTI_String;	};
template<> struct PMap<double>		{ static constexpr aiPropertyTypeInfo value = aiPTI_Double;	};
template<> struct PMap<char>		{ static constexpr aiPropertyTypeInfo value = aiPTI_Buffer;	};

class MaterialProperty
{
public: 
	MaterialProperty(std::string pKey, unsigned int indx, const aiPropertyTypeInfo& propertyType, const char* data_, unsigned int dataSize);
	const std::vector<unsigned char>& GetData() const;
	const size_t GetSize() const;
	aiPropertyTypeInfo GetType() const noexcept;
	std::string GetKey() const noexcept;
	void ShowGUI() noexcept;
private:
	std::string pKey;
	aiPropertyTypeInfo propertyType;
	unsigned int indx;
	std::vector<unsigned char> data;
};