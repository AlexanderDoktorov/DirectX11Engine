#include "LightSource.h"

std::unique_ptr<PixelConstantBuffer<LightDesc>> LightSource::pLightBuffer{};

LightSource::LightSource(Graphics& Gfx) 
	:
	Model(Gfx, R"(G:\Visual Studio Projects\ProjForTests\Models\bulb\bulb.obj)", aiProcess_Triangulate)
{
	if (!pLightBuffer)
	{
		pLightBuffer = std::make_unique<PixelConstantBuffer<LightDesc>>(Gfx);
		pLightBuffer->SetBindSlot(1U);
	}

	Reset();
}

void LightSource::ShowControlChildWindow()
{
	ImGui::BeginChild("Light control", ImVec2(600, 200), true);
	{
		ImGui::SliderFloat3("XYZ", &lightDesc.pos.x, -200.f, 200.f, "%.4f");
		ImGui::SliderFloat3("Diffuse color RGB", &lightDesc.diffuseColor.x, 0.f, 1.f);
		ImGui::SliderFloat("Catt", &lightDesc.Catt, 0.f, 1.f, "%.4f");
		ImGui::SliderFloat("Latt", &lightDesc.Latt, 0.f, 1.f, "%.4f");
		ImGui::SliderFloat("Qatt", &lightDesc.Qatt, 0.f, 1.f, "%.4f");
		ImGui::SliderFloat("Diffuse intensity", &lightDesc.diffuseIntensity, 0.f, 10.f), "%.4f";

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
	Model::SetPostion(_x, _y, _z);
	lightDesc.pos = { _x,_y,_z };
}

DirectX::XMFLOAT3 LightSource::GetPosition() const noexcept
{
	//DirectX::XMVECTOR vCenter = DirectX::XMVectorSet(0.f,0.f,0.f,0.f);
	//vCenter = DirectX::XMVector3Transform(vCenter, GetTransform());
	//DirectX::XMFLOAT3 Position3D;
	//DirectX::XMStoreFloat3(&Position3D, vCenter);
	return lightDesc.pos;
}

void LightSource::Reset()
{
	Model::SetPostion(0.f, 0.f, -10.f);
	lightDesc =
	{
		{ 0.f, 0.f, -10.f },
		{ 1.0f,1.0f,1.0f },
		62.0f,
		34.0f,
		6.7f,
		0.0075f,
	};
}

void LightSource::Bind(Graphics& Gfx) noexcept
{
	// lightDesc.pos - точка в центре шара (светильника)
	lightDesc.pos = GetPosition();
	pLightBuffer->Update(Gfx, lightDesc);
	pLightBuffer->Bind(Gfx);
}

void LightSource::Draw(Graphics& Gfx)
{
	Model::Draw(Gfx);
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

void LightSource::Update(float dt) noexcept
{
	// TO DO
}

DirectX::XMMATRIX LightSource::GetTransform() const noexcept
{
	return  DirectX::XMMatrixTranslation(lightDesc.pos.x, lightDesc.pos.y, lightDesc.pos.z);
}

LightDesc LightSource::GetDesc() const noexcept
{
	return lightDesc;
}
