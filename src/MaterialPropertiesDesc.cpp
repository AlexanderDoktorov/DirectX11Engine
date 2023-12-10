#include "MaterialPropertiesDesc.h"
#include "imgui.h"

void MaterialPropertiesDesc::AddProperty(const MaterialProperty& property)
{
	properties.push_back(property);
}

std::optional<MaterialProperty> MaterialPropertiesDesc::GetProperty(std::string pKey, unsigned int, unsigned int) const noexcept
{
	for (auto& p : properties)
	{
		if (p.GetKey() == pKey)
			return p;
	}
	return std::nullopt;
}

bool MaterialPropertiesDesc::ShowGUI(const char* label) noexcept
{
	if (ImGui::BeginCombo(label, properties.empty() ? "No properties" : properties[selectedPropertyIndex].GetKey().c_str())) {
		for (size_t i = 0; i < properties.size(); i++) {
			// Check if the current item is selected
			const bool isSelected = (selectedPropertyIndex == i);

			// Display the current item in the combo box
			if (ImGui::Selectable(properties[i].GetKey().c_str(), isSelected))
				selectedPropertyIndex = i; // Set the selected item index

			// If the current item is selected, set the combo box highlight
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (bool changed = properties[selectedPropertyIndex].ShowGUI())
	{
		return changed;
	}
	return false;
}
