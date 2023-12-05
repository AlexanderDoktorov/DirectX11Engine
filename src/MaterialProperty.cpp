#include "MaterialProperty.h"

MaterialProperty::MaterialProperty(std::string pKey, unsigned int indx, const aiPropertyTypeInfo& propertyType, const char* data_, unsigned int dataSize)
	:
	pKey(pKey),
	propertyType(propertyType),
	indx(indx)
{
	data.resize(dataSize);
	std::memcpy(data.data(), data_, dataSize);
}

const std::vector<unsigned char>& MaterialProperty::GetData() const
{
	return data;
}

const size_t MaterialProperty::GetSize() const
{
	return data.size();
}

aiPropertyTypeInfo MaterialProperty::GetType() const noexcept
{
	return propertyType;
}

std::string MaterialProperty::GetKey() const noexcept
{
	return pKey;
}
