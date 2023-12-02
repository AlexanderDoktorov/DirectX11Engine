#include "MaterialProperty.h"

void* MaterialProperty::GetData() const noexcept
{
	return data;
}

bool MaterialProperty::Empty() const noexcept
{
	return data == nullptr;
}

EMATERIAL_PROPERTY MaterialProperty::GetPropertyType() const noexcept
{
	return materialProperty;
}

MaterialProperty::operator bool() const noexcept
{
	return !Empty();
}

MaterialProperty::~MaterialProperty()
{
	if (data)
	{
		delete data;
		data = nullptr;
	}
}