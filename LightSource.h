#pragma once
#include "IBindable.h"
#include "LightConstantBuffer.h"
#include "imgui.h"
#include "SolidBall.h"

class LightSource
{
	struct LightData
	{
		alignas(16) dx::XMFLOAT3 pos = dx::XMFLOAT3();
		alignas(16) dx::XMFLOAT3 ambient = dx::XMFLOAT3();
		alignas(16) dx::XMFLOAT3 diffuseColor = dx::XMFLOAT3();
		float diffuseIntensity = 1.f;
		float Catt = 1.f;
		float Latt = 1.f;
		float Qatt = 1.f;
	};
public:
	LightSource(Graphics& Gfx) 
		: 
		pBuffer(Gfx),
		mesh(Gfx)
	{
		mesh.SetScale(dx::XMFLOAT3(0.5, 0.5, 0.5));
		Reset();
	}

	void Draw(Graphics& Gfx) noexcept
	{
		mesh.SetPosition(lightData.pos);
		mesh.Draw(Gfx);
	}

	void Bind(Graphics& Gfx) noexcept
	{
		LightData cpy = lightData;
		
		auto ViewMatrix = Gfx.GetCamera().GetCameraMatrix();
		dx::XMStoreFloat3(&cpy.pos ,dx::XMVector3Transform(dx::XMLoadFloat3(&cpy.pos), ViewMatrix));

		pBuffer.Update(Gfx, cpy);
		pBuffer.Bind(Gfx);
	}

	void Reset()
	{
		lightData = {
			{ 0.0f,7.0f,0.0f },
			{ 0.05f,0.05f,0.05f },
			{ 1.0f,1.0f,1.0f },
			1.0f,
			1.0f,
			0.045f,
			0.0075f,
		};
	}

	void ShowControlWindow()
	{
		if(ImGui::Begin("Light control"))
		{
			ImGui::SliderFloat3("XYZ", &lightData.pos.x, -200.f, 200.f);
			ImGui::SliderFloat3("Ambiend RGB", &lightData.ambient.x, 0.f, 1.f);
			ImGui::SliderFloat3("Diffuse color RGB", &lightData.diffuseColor.x, 0.f, 1.f);
			ImGui::SliderFloat("Catt", &lightData.Catt, 0.f, 1.f);
			ImGui::SliderFloat("Latt", &lightData.Latt, 0.f, 1.f);
			ImGui::SliderFloat("Qatt", &lightData.Qatt, 0.f, 1.f);
			ImGui::SliderFloat("Diffuse intensity", &lightData.diffuseIntensity, 0.f, 10.f);
			if(ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();
	}
private:
	LightData lightData;
	SolidBall mesh;
	LightConstantBuffer<LightData> pBuffer;
};