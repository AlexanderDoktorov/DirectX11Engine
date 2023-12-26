#pragma once
#include <DirectXMath.h>
#include <memory>

#include "IMovable.h"
#include "IBindable.h"
#include "imgui.h"
#include "PixelConstantBuffer.h"

namespace dx = DirectX;

enum LIGHT_TYPE : uint32_t
{
	LIGHT_TYPE_POINT_LIGHT = 0,
	LIGHT_TYPE_SPOTLIGHT = 1,
	LIGHT_TYPE_DIRECTIONAL = 2
};

class Light : public IBindable
{
protected:
	struct Data
	{
		// World position
		alignas(16) dx::XMFLOAT3 worldPosition = dx::XMFLOAT3(0.f,10.f,0.f);
		// Color
		alignas(16) dx::XMFLOAT3 ambientColor  = dx::XMFLOAT3(0.05f,0.05f,0.05f);
		alignas(16) dx::XMFLOAT3 diffuseColor  = dx::XMFLOAT3(1.f,1.f,1.f); // White
		alignas(16) dx::XMFLOAT3 specularColor = dx::XMFLOAT3(1.f,1.f,1.f); // White
		// Intensity
		float ambientIntensity = 0.05f;
		float diffuseIntensity = 1.f;
		float specularIntensity = 0.5f;
		// Atteniation
		float Catt = 1.f;
		float Latt = 0.045f;
		float Qatt = 0.0075f;
		// Spotlight
		alignas(16) dx::XMFLOAT3 spotlightWorldDirection = dx::XMFLOAT3(0.f,0.f,1.f); // along Z-axis by default 
		float spotlightAngle = dx::XM_PI / 5.f; // defaults to 36 degrees
		// TypeId
		LIGHT_TYPE typeId = LIGHT_TYPE_POINT_LIGHT;
	} data{};
public:
	using data_type   = Data;
	using buffer_type = PixelConstantBuffer<data_type>;
	static constexpr inline UINT light_desc_bind_slot = 1U;
public:
	Light(Graphics& Gfx, LIGHT_TYPE typeID)
	{ 
		SetLightType(typeID);
		if (!pLightBuffer)
		{
			pLightBuffer = std::make_unique<buffer_type>(Gfx, data);
			pLightBuffer->SetBindSlot(light_desc_bind_slot);
		}
	}

	bool ShowLightGUI() noexcept;

	LIGHT_TYPE  GetLightType() const noexcept { return data.typeId; }
	const auto& GetDesc()      const noexcept { return data; }

			void SetLightType(const LIGHT_TYPE& typeId) noexcept { data.typeId = typeId; }
	virtual void Reset()								noexcept { data = Data(); } // resets to default;
	virtual void Bind(Graphics& Gfx)					noexcept override;

private:
	static std::unique_ptr<buffer_type> pLightBuffer;
};