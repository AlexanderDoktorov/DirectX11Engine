#include "Light.h"
#include "SlotLayout.h"

RuntimeBuffer::PixelConstantBufferEx Light::constantBuffer{};

Light::Light(Graphics& Gfx, LIGHT_TYPE typeID)
{ 
	SetLightType(typeID);
	// Fill buffer
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

	dataBuff = { layout };
	dataBuff["worldPosition"]	  = dx::XMFLOAT3(0.f,10.f,0.f);
	dataBuff["ambientColor"]	  = dx::XMFLOAT3(0.05f,0.05f,0.05f);
	dataBuff["diffuseColor"]	  = dx::XMFLOAT3(1.f,1.f,1.f); // White
	dataBuff["specularColor"]	  = dx::XMFLOAT3(1.f,1.f,1.f); // White
	dataBuff["ambientIntensity"]  = 0.05f;
	dataBuff["diffuseIntensity"]  = 1.f;
	dataBuff["specularIntensity"] = 0.5f;
	dataBuff["Catt"] = 1.f;
	dataBuff["Latt"] = 0.045f;
	dataBuff["Qatt"] = 0.0075f;
	dataBuff["spotlightWorldDirection"] = dx::XMFLOAT3(0.f,0.f,1.f);
	dataBuff["spotlightAngle"] = dx::XM_PI / 5.f;
	dataBuff["typeId"] = (int)LIGHT_TYPE_POINT_LIGHT;
	if (!constantBuffer)
		constantBuffer = { Gfx, dataBuff.GetLayout(), nullptr, SLOT_BUFFER_LIGHT};
}

void Light::Bind(Graphics& Gfx) noexcept
{
	constantBuffer.Update(Gfx, &dataBuff);
	constantBuffer.Bind(Gfx);
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
	changed |= ImGui::SliderFloat("Ambient intensity", &dataBuff["ambientIntensity"], 0.f, 100.f, "%.5f");
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