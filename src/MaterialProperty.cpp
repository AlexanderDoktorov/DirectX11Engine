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

bool MaterialProperty::ShowGUI(std::string hash) noexcept
{
	bool updated = false;
	std::string str = pKey;
	const char* label = str.append("##").append(hash).c_str();
	switch (propertyType)
	{
	case aiPTI_Float:
		if (data.size() == sizeof(float))
			updated |= ImGui::InputFloat(label, reinterpret_cast<float*>(data.data()));
		if (data.size() == 3 * sizeof(float))
		{
			if (pKey.substr(0, 4) == "$clr")
				updated |= ImGui::ColorEdit3(label, reinterpret_cast<float*>(data.data()));
			else
				updated |= ImGui::SliderFloat3(label, reinterpret_cast<float*>(data.data()), -10.f, 10.f);
		}
		break;
	case aiPTI_Integer:
		if (data.size() == sizeof(int))
			updated |= ImGui::SliderInt(label, reinterpret_cast<int*>(data.data()), 0, 100);
		break;
	case aiPTI_String:
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 1.f, 1.f, 1.f));
		ImGui::TextUnformatted((const char*)data.data());
		ImGui::PopStyleColor();
		break;
	}
	return updated;
}
