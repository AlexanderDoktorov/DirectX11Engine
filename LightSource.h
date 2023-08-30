#pragma once
#include "IBindable.h"
#include "LightConstantBuffer.h"
#include "imgui.h"
#include "SolidBall.h"
#include <array>

#define MAX_NUM_LIGTHS 20U

class LightSource : public IPlacableItem, public IToString
{
public:
	struct LightDesc
	{
		alignas(16) dx::XMFLOAT3 pos = dx::XMFLOAT3();
		alignas(16) dx::XMFLOAT3 diffuseColor = dx::XMFLOAT3();
		float diffuseIntensity = 1.f;
		float Catt = 1.f;
		float Latt = 1.f;
		float Qatt = 1.f;
	};

	struct LightBuffer
	{
		LightBuffer() = default;
		LightBuffer(const std::vector<LightDesc>& descs)
			:
			NUM_LIGHTS(descs.size())
		{
			std::copy_n(descs.begin(), descs.size(), lightDescs.begin());
		}


		alignas(16) unsigned int NUM_LIGHTS = 0U;
		std::array<LightDesc, MAX_NUM_LIGTHS> lightDescs;
	};

public:
	LightSource(Graphics& Gfx)
		:
		mesh(Gfx)
	{
		if(!p_light_constant_buffer)
			p_light_constant_buffer =  std::make_unique<LightConstantBuffer<LightBuffer>>(Gfx);
		light_num++;

		mesh.SetScale(dx::XMFLOAT3(0.5, 0.5, 0.5));
		Reset();
	}

	void Draw(Graphics& Gfx) noexcept
	{
		mesh.SetWorldPosition(lightDesc.pos);
		mesh.Draw(Gfx);
	}

	static void BindLights(Graphics& Gfx, std::vector<LightDesc> light_descs /* copy */ ) noexcept
	{
		assert(p_light_constant_buffer != nullptr);

		for (auto& light_desc : light_descs)
		{
			auto ViewMatrix = Gfx.GetCamera().GetCameraMatrix();
			dx::XMStoreFloat3(&light_desc.pos, dx::XMVector3Transform(dx::XMLoadFloat3(&light_desc.pos), ViewMatrix));
		}

		LightSource::p_light_constant_buffer->Update(Gfx, LightBuffer(light_descs));
		LightSource::p_light_constant_buffer->Bind(Gfx);
	}

	void Reset()
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

	LightDesc GetDesc() const noexcept { return lightDesc; }

	void ShowControlChildWindow()
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


	// IPlacableItem
	virtual void SetWorldPosition(const dx::XMFLOAT3& new_Wpos) override;
	virtual dx::XMFLOAT3 GetWorldPosition() const noexcept override;

	// IToString
	const char* ToString() const noexcept override;

private:
	static std::unique_ptr<LightConstantBuffer<LightBuffer>> p_light_constant_buffer;

	LightDesc lightDesc;
	SolidBall mesh;
	static unsigned int light_num;
};