#include "LightSource.h"

std::unique_ptr<PixelConstantBuffer<LightDesc>> LightSource::pLightBuffer{};

LightSource::LightSource(Graphics& Gfx) 
	:
	Model(Gfx, R"(.\Models\bulb\bulb.obj)", aiProcess_Triangulate)
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
		ImGui::SliderFloat3("XYZ", &lightDesc.worldPosition.x, -200.f, 200.f, "%.4f");
		ImGui::ColorEdit3("Ambient color RGB", &lightDesc.ambientColor.x);
		ImGui::ColorEdit3("Diffuse color RGB", &lightDesc.diffuseColor.x);
		ImGui::ColorEdit3("Specular color RGB", &lightDesc.specularColor.x);
		ImGui::SliderFloat("Catt", &lightDesc.Catt, 0.f, 1.f, "%.4f");
		ImGui::SliderFloat("Latt", &lightDesc.Latt, 0.f, 1.f, "%.4f");
		ImGui::SliderFloat("Qatt", &lightDesc.Qatt, 0.f, 1.f, "%.4f");
		ImGui::SliderFloat("Ambient intensity", &lightDesc.ambientIntensity, 0.f, 100.f, "%.5f");
		ImGui::SliderFloat("Diffuse intensity", &lightDesc.diffuseIntensity, 0.f, 100.f, "%.5f");
		ImGui::SliderFloat("Specular intensity", &lightDesc.specularIntensity, 0.f, 100.f, "%.5f");

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
	Model::SetRootTransform(dx::XMMatrixTranslation(_x, _y, _z));
	lightDesc.worldPosition = { _x,_y,_z };
}

DirectX::XMFLOAT3 LightSource::GetPosition() const noexcept
{
	//DirectX::XMVECTOR vCenter = DirectX::XMVectorSet(0.f,0.f,0.f,0.f);
	//vCenter = DirectX::XMVector3Transform(vCenter, GetTransform());
	//DirectX::XMFLOAT3 Position3D;
	//DirectX::XMStoreFloat3(&Position3D, vCenter);
	return lightDesc.worldPosition;
}

void LightSource::Reset()
{
	LightDesc defaultDesc{};
	defaultDesc.worldPosition = dx::XMFLOAT3(0.f,10.f,0.f);
	defaultDesc.ambientColor  = dx::XMFLOAT3(0.05f,0.05f,0.05f);
	defaultDesc.diffuseColor  = dx::XMFLOAT3(1.f,1.f,1.f); // White
	defaultDesc.specularColor = dx::XMFLOAT3(1.f,1.f,1.f); // White
	defaultDesc.ambientIntensity = 0.05f;
	defaultDesc.diffuseIntensity = 1.f;
	defaultDesc.specularIntensity = 0.5f;
	defaultDesc.Catt = 1.f;
	defaultDesc.Latt = 0.045f;
	defaultDesc.Qatt = 0.0075f;
	lightDesc = defaultDesc;
	Model::SetRootTransform(
		dx::XMMatrixTranslation
		(
			lightDesc.worldPosition.x, 
			lightDesc.worldPosition.y, 
			lightDesc.worldPosition.z
		)
	);
}

void LightSource::Bind(Graphics& Gfx) noexcept
{
	pLightBuffer->Update(Gfx, lightDesc);
	pLightBuffer->Bind(Gfx);
}

void LightSource::Draw(Graphics& Gfx) const noexcept 
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
	return  DirectX::XMMatrixTranslation(lightDesc.worldPosition.x, lightDesc.worldPosition.y, lightDesc.worldPosition.z);
}

LightDesc LightSource::GetDesc() const noexcept
{
	return lightDesc;
}

void LightSource::SetDesc(const LightDesc& lightDesc) noexcept
{
	this->lightDesc = lightDesc;
}


