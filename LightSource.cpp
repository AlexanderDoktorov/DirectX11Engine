#include "LightSource.h"

std::unique_ptr<LightConstantBuffer<LightDesc>> LightSource::pLightBuffer{};

LightSource::LightSource(Graphics& Gfx) :
	mesh(Gfx)
{
	if (!pLightBuffer)
		pLightBuffer = std::make_unique<LightConstantBuffer<LightDesc>>(Gfx);

	mesh.SetScale(dx::XMFLOAT3(0.5, 0.5, 0.5));
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

void LightSource::Bind(Graphics& Gfx)
{
	pLightBuffer->Update(Gfx, lightDesc);
	pLightBuffer->Bind(Gfx);
}

void LightSource::Draw(Graphics& Gfx) noexcept
{
	mesh.SetWorldPosition(lightDesc.pos);
	mesh.Draw(Gfx);
}

void LightSource::DrawIntoGBuffer(Graphics& Gfx) noexcept
{
	mesh.SetWorldPosition(lightDesc.pos);
	mesh.DrawIntoGBuffer(Gfx);
}

void LightSource::DrawIntoLightRenderTarget(Graphics& Gfx) noexcept
{
	Gfx.BindLightBufferAsRenderTarget();
	Gfx.Draw(3U);
}

void LightSource::SetWorldPosition(const dx::XMFLOAT3& new_Wpos)
{
	lightDesc.pos = new_Wpos;
}

dx::XMFLOAT3  LightSource::GetWorldPosition() const noexcept
{
	return lightDesc.pos;
}

const char* LightSource::ToString() const noexcept
{
	return "Light Source";
}