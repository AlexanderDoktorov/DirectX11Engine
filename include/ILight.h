#pragma once
#include <DirectXMath.h>
#include "IMovable.h"
#include <memory>
#include "imgui.h"
#include "PixelConstantBuffer.h"
#include "DOK_traits.h"

namespace dx = DirectX;

class ILight
{
public:
	virtual ~ILight() = default;

	virtual bool ShowLightGUI() noexcept = 0;
	virtual void Reset() noexcept = 0;
};

class PointLightDesc : public ILight
{
public:

	bool ShowLightGUI() noexcept override
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

		ImVec4 button_hovered_color = ImVec4(0.f, 0.f, 1.f, 1.f);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
		if (ImGui::Button("Reset light properties"))
			Reset();
		ImGui::PopStyleColor(1U);

		return changed;
	}

	void Reset() noexcept override { data = Data(); } // resets to default

protected:
	struct Data
	{
		alignas(16) dx::XMFLOAT3 worldPosition = dx::XMFLOAT3(0.f,10.f,0.f);
		alignas(16) dx::XMFLOAT3 ambientColor  = dx::XMFLOAT3(0.05f,0.05f,0.05f);
		alignas(16) dx::XMFLOAT3 diffuseColor  = dx::XMFLOAT3(1.f,1.f,1.f); // White
		alignas(16) dx::XMFLOAT3 specularColor = dx::XMFLOAT3(1.f,1.f,1.f); // White
		float ambientIntensity = 0.05f;
		float diffuseIntensity = 1.f;
		float specularIntensity = 0.5f;
		float Catt = 1.f;
		float Latt = 0.045f;
		float Qatt = 0.0075f;
	} data{};
};

// other types specialization

template<class T>
class LightBase : public T
{
public:
	using data_type = T::Data;
	using desc_type = T;
	using buffer_type = typename PixelConstantBuffer<data_type>;

	const desc_type& GetDesc() const noexcept { return static_cast<T&>(*this) ;  }

	const data_type& GetData() const noexcept { return T::data; }
	void SetData(const data_type& data) noexcept { T::data = data; }
	
	virtual ~LightBase() = default;

protected:
	static std::unique_ptr<buffer_type> pLightBuffer;
};

template<class T>
std::unique_ptr<typename LightBase<T>::buffer_type> LightBase<T>::pLightBuffer{};