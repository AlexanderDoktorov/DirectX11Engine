#include "Light.h"

std::shared_ptr<Light::buffer_type> Light::pLightBuffer{};

Light::Light(Graphics& Gfx, LIGHT_TYPE typeID)
{ 
	SetLightType(typeID);
	if (!pLightBuffer)
		pLightBuffer = buffer_type::Resolve(Gfx, data, light_desc_bind_slot);
}

void Light::Bind(Graphics& Gfx) noexcept
{
	pLightBuffer->Update(Gfx, data);
	pLightBuffer->Bind(Gfx);
}

bool Light::ShowLightGUI() noexcept
{
	bool changed = false;
	changed |= ImGui::ColorEdit3("Ambient color RGB", &data.ambientColor.x);
	changed |= ImGui::ColorEdit3("Diffuse color RGB", &data.diffuseColor.x);
	changed |= ImGui::ColorEdit3("Specular color RGB", &data.specularColor.x);
	changed |= ImGui::SliderFloat("Catt", &data.Catt, 0.f, 10.f, "%.4f");
	changed |= ImGui::SliderFloat("Latt", &data.Latt, 0.f, 10.f, "%.4f");
	changed |= ImGui::SliderFloat("Qatt", &data.Qatt, 0.f, 10.f, "%.4f");
	changed |= ImGui::SliderFloat("Ambient intensity", &data.ambientIntensity, 0.f, 100.f, "%.5f");
	changed |= ImGui::SliderFloat("Diffuse intensity", &data.diffuseIntensity, 0.f, 100.f, "%.5f");
	changed |= ImGui::SliderFloat("Specular intensity", &data.specularIntensity, 0.f, 100.f, "%.5f");

	if (data.typeId == LIGHT_TYPE_SPOTLIGHT)
	{
		changed |= ImGui::SliderAngle("Spotlight angle", &data.spotlightAngle);
		changed |= ImGui::SliderFloat3("Spotlight world direction", &data.spotlightWorldDirection.x, -1.f, 1.f, "%.6f");
	}

	ImVec4 button_hovered_color = ImVec4(0.f, 0.f, 1.f, 1.f);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
	if (ImGui::Button("Reset light properties"))
		Reset();
	ImGui::PopStyleColor(1U);

	return changed;
}