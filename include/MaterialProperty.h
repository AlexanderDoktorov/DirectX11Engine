#pragma once
#include <DirectXMath.h>
#include <memory>
#include "assimp\material.h"

#define _PART_SPEC_ template<>

enum EMATERIAL_PROPERTY
{
	EMATERIAL_PROPERTY_UNDEFINED,
	EMATERIAL_PROPERTY_COLOR_DIFFUSE,
	EMATERIAL_PROPERTY_COLOR_SPECULAR,
	EMATERIAL_PROPERTY_COLOR_AMBIENT,
	EMATERIAL_PROPERTY_SHININESS,
	EMATERIAL_PROPERTY_SHADING_MODEL,
	EMATERIAL_PROPERTY_COLOR_REFLECTIVE,
};

struct matInfo
{
	const char* pKey;
	unsigned int type;
	unsigned int idx;
};

struct COLOR3_TYPE
{
	using type = aiColor3D;
};

struct FLOAT_TYPE
{
	using type = float;
};

struct INTEGER_TYPE
{
	using type = int;
};

// property data type
template<EMATERIAL_PROPERTY prop>
struct PDT
{};

_PART_SPEC_
struct PDT<EMATERIAL_PROPERTY_COLOR_DIFFUSE> : COLOR3_TYPE
{
	static constexpr matInfo value = { AI_MATKEY_COLOR_DIFFUSE };
};
_PART_SPEC_
struct PDT<EMATERIAL_PROPERTY_COLOR_SPECULAR> : COLOR3_TYPE
{
	static constexpr matInfo value = { AI_MATKEY_COLOR_SPECULAR };
};
_PART_SPEC_
struct PDT<EMATERIAL_PROPERTY_COLOR_AMBIENT> : COLOR3_TYPE
{
	static constexpr matInfo value = { AI_MATKEY_COLOR_AMBIENT };
};
_PART_SPEC_
struct PDT<EMATERIAL_PROPERTY_SHININESS> : FLOAT_TYPE
{
	static constexpr matInfo value = { AI_MATKEY_SHININESS };
};
_PART_SPEC_
struct PDT<EMATERIAL_PROPERTY_SHADING_MODEL> : INTEGER_TYPE
{
	static constexpr matInfo value = { AI_MATKEY_SHADING_MODEL };
};
_PART_SPEC_
struct PDT<EMATERIAL_PROPERTY_COLOR_REFLECTIVE> : COLOR3_TYPE
{
	static constexpr matInfo value = { AI_MATKEY_COLOR_REFLECTIVE };
};

class MaterialProperty
{
	friend class Material;
	struct constuctor_enabler;
public:
	MaterialProperty(const MaterialProperty&) = delete;
	const MaterialProperty operator=(const MaterialProperty&) = delete;

	template<class T>
	explicit MaterialProperty(constuctor_enabler enabler, T* data, EMATERIAL_PROPERTY P) : materialProperty(P)
	{
		this->data = new unsigned char[sizeof(T)];
		memcpy(this->data, data, sizeof(T));
	}

	MaterialProperty(MaterialProperty&& matp) noexcept
	{
		std::swap(matp.data, this->data);
		std::swap(matp.materialProperty, this->materialProperty);
	}

	template<EMATERIAL_PROPERTY P, class dataType = PDT<P>::type, const auto& MI = PDT<P>::value>
	static std::shared_ptr<MaterialProperty> Make(aiMaterial* pMaterial)
	{
		if (pMaterial)
		{
			dataType pData{};
			if (pMaterial->Get(MI.pKey, MI.type, MI.idx, pData) == aiReturn_FAILURE)
				return nullptr;
			else
				return std::make_shared<MaterialProperty>(constuctor_enabler{0}, &pData, P);
		}
		else
		{
			return nullptr;
		}
	}

	void*				GetData() const noexcept;
	bool				Empty() const noexcept;
	EMATERIAL_PROPERTY	GetPropertyType() const noexcept;

	explicit operator bool() const noexcept;

	~MaterialProperty();
private:
	unsigned char* data = nullptr;
	EMATERIAL_PROPERTY materialProperty = EMATERIAL_PROPERTY_UNDEFINED;
private:
	struct constuctor_enabler
	{
		explicit constuctor_enabler(int) {}
	};
};