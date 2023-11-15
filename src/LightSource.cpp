#include "LightSource.h"

std::unique_ptr<LightConstantBuffer<LightDesc>> LightSource::pLightBuffer{};

LightSource::LightSource(Graphics& Gfx) :
	SolidBall(Gfx)
{
	if (!pLightBuffer)
		pLightBuffer = std::make_unique<LightConstantBuffer<LightDesc>>(Gfx);

	SolidBall::SetScale(dx::XMFLOAT3(0.5, 0.5, 0.5));
	Reset();
}

void LightSource::ShowControlChildWindow()
{
	ImGui::BeginChild("Light control", ImVec2(600, 200), true);
	{
		ImGui::SliderFloat3("XYZ", &lightDesc.pos.x, -200.f, 200.f);
		ImGui::SliderFloat3("Diffuse color RGB", &lightDesc.diffuseColor.x, 0.f, 1.f);
		ImGui::SliderFloat("Catt", &lightDesc.Catt, 0.f, 1.f);
		ImGui::SliderFloat("Latt", &lightDesc.Latt, 0.f, 1.f);
		ImGui::SliderFloat("Qatt", &lightDesc.Qatt, 0.f, 1.f);
		ImGui::SliderFloat("Diffuse intensity", &lightDesc.diffuseIntensity, 0.f, 10.f);

		ImVec4 button_hovered_color = ImVec4(0.f, 0.f, 1.f, 1.f);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
		if (ImGui::Button("Reset"))
			Reset();
		ImGui::PopStyleColor(1U);
	}
	ImGui::EndChild();
}

void LightSource::SetPosition(float _x, float _y, float _z)
{
	lightDesc.pos = dx::XMFLOAT3(_x,_y,_z);
}

DirectX::XMFLOAT3 LightSource::GetPosition() const noexcept
{
	return lightDesc.pos;
}

void LightSource::Reset()
{
	lightDesc =
	{
		{ 0.0f,7.0f,0.0f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void LightSource::Bind(Graphics& Gfx) noexcept
{
	pLightBuffer->Update(Gfx, lightDesc);
	pLightBuffer->Bind(Gfx);
}

void LightSource::Draw(Graphics& Gfx)
{
	SolidBall::SetPosition(lightDesc.pos.x, lightDesc.pos.y, lightDesc.pos.z);
	SolidBall::Draw(Gfx);
}

const char* LightSource::ToString() const noexcept
{
	return "Light Source";
}

void LightSource::SetDiffuseColor(const dx::XMFLOAT3& diffuseColor) noexcept
{
	lightDesc.diffuseColor = diffuseColor;
}

void LightSource::SetDiffuseIntensity(float diffuseIntensity) noexcept
{
	lightDesc.diffuseIntensity = diffuseIntensity;
}

void LightSource::SetConstantAttenuation(const float& Catt) noexcept
{
	lightDesc.Catt = Catt;
}

void LightSource::SetLinearAttenuation(const float& Latt) noexcept
{
	lightDesc.Latt = Latt;
}

void LightSource::SetQuadAttenuation(const float& Qatt) noexcept
{
	lightDesc.Qatt = Qatt;
}

LightDesc LightSource::GetDesc() const noexcept
{
	return lightDesc;
}
