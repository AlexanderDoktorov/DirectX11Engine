#include "Light.h"
#include "SlotLayout.h"
#include "RuntimeBuffer.h"

std::shared_ptr<Light::buffer_type> Light::pLightBuffer{};
RuntimeBuffer::CachingPixelConstantBufferEx Light::LightBuffer{};
RuntimeBuffer::Buffer Light::buffer{};

Light::Light(Graphics& Gfx, LIGHT_TYPE typeID)
{ 
	SetLightType(typeID);
	if (!LightBuffer)
	{
		RuntimeBuffer::BufferLayout layout{};
		layout.AddField<DirectX::XMFLOAT3>("worldPosition");
		layout.AddField<DirectX::XMFLOAT3>("ambientColor");
		layout.AddField<DirectX::XMFLOAT3>("diffuseColor");
		layout.AddField<DirectX::XMFLOAT3>("specularColor");
		layout.AddField<float>("ambientIntensity");
		layout.AddField<float>("diffuseIntensity");
		layout.AddField<float>("specularIntensity");
		layout.AddField<float>("Catt");
		layout.AddField<float>("Latt");
		layout.AddField<float>("Qatt");
		layout.AddField<DirectX::XMFLOAT3>("spotlightWorldDirection");
		layout.AddField<float>("spotlightAngle");
		layout.AddField<int>("typeId");
		layout.AlignAs16();

		buffer = { layout };
		buffer["worldPosition"]	  = dx::XMFLOAT3(0.f,10.f,0.f);
		buffer["ambientColor"]	  = dx::XMFLOAT3(0.05f,0.05f,0.05f);
		buffer["diffuseColor"]	  = dx::XMFLOAT3(1.f,1.f,1.f); // White
		buffer["specularColor"]	  = dx::XMFLOAT3(1.f,1.f,1.f); // White
		buffer["ambientIntensity"]  = 0.05f;
		buffer["diffuseIntensity"]  = 1.f;
		buffer["specularIntensity"] = 0.5f;
		buffer["Catt"] = 1.f;
		buffer["Latt"] = 0.045f;
		buffer["Qatt"] = 0.0075f;
		buffer["spotlightWorldDirection"] = dx::XMFLOAT3(0.f,0.f,1.f);
		buffer["spotlightAngle"] = dx::XM_PI / 5.f;
		buffer["typeId"] = (int)LIGHT_TYPE_POINT_LIGHT;
		LightBuffer = { Gfx, &buffer, SLOT_BUFFER_LIGHT };
	}
}

void Light::Bind(Graphics& Gfx) noexcept
{
	LightBuffer.Update(Gfx);
	LightBuffer.Bind(Gfx);
}

bool Light::ShowLightGUI()
{
	bool changed = false;
	changed |= ImGui::ColorEdit3("Ambient color RGB", &data.ambientColor.x);
	changed |= ImGui::ColorEdit3("Diffuse color RGB", &data.diffuseColor.x);
	changed |= ImGui::ColorEdit3("Specular color RGB", &data.specularColor.x);
	changed |= ImGui::SliderFloat("Catt", &data.Catt, 0.f, 10.f, "%.4f");
	changed |= ImGui::SliderFloat("Latt", &data.Latt, 0.f, 10.f, "%.4f");
	changed |= ImGui::SliderFloat("Qatt", &data.Qatt, 0.f, 10.f, "%.4f");
	changed |= ImGui::SliderFloat("Ambient intensity", &(float&)buffer["ambientIntensity"], 0.f, 100.f, "%.5f");
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