#include "MaterialProperty.h"

MaterialProperty::MaterialProperty(std::string pKey, unsigned int indx, const aiPropertyTypeInfo& propertyType, const char* data_, unsigned int dataSize)
	:
	pKey(pKey),
	propertyType(propertyType),
	indx(indx)
{

	if (propertyType == aiPTI_String)
	{
		char placeholder = 'X';
		std::string myString{ data_,dataSize};
		size_t lastNullPos = myString.find_last_of('\0');

		for (size_t i = 0; i < myString.size(); ++i) {
			if (myString[i] == '\0' && i != lastNullPos) {
				myString[i] = placeholder;
			}
		}
		// Erase all placeholder characters
		myString.erase(std::remove(myString.begin(), myString.end(), placeholder), myString.end());
		
		data.resize(myString.size());
		std::memcpy(data.data(), myString.data(), myString.size());
	}
	else
	{
		data.resize(dataSize);
		std::memcpy(data.data(), data_, dataSize);
	}
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

void MaterialProperty::ShowGUI() noexcept
{
	switch (propertyType)
	{
	case aiPTI_Float:
		if (data.size() == sizeof(float))
			ImGui::SliderFloat(pKey.c_str(), reinterpret_cast<float*>(data.data()), 0.f, 10.f);
		if (data.size() == 3 * sizeof(float))
		{
			if (pKey.substr(0, 4) == "$clr")
				ImGui::ColorEdit3(pKey.c_str(), reinterpret_cast<float*>(data.data()));
			else
				ImGui::SliderFloat3(pKey.c_str(), reinterpret_cast<float*>(data.data()), -10.f, 10.f);
		}
		break;
	case aiPTI_Integer:
		if (data.size() == sizeof(int))
			ImGui::SliderInt(pKey.c_str(), reinterpret_cast<int*>(data.data()), 0, 100);
		break;
	case aiPTI_String:
		ImGui::Text((const char*)data.data());
		break;
	}
}
