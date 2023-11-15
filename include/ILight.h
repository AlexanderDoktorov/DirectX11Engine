#pragma once
#include <DirectXMath.h>

namespace dx = DirectX;

struct LightDesc
{
	alignas(16) dx::XMFLOAT3 pos = dx::XMFLOAT3();
	alignas(16) dx::XMFLOAT3 diffuseColor = dx::XMFLOAT3();
	float diffuseIntensity = 1.f;
	float Catt = 1.f;
	float Latt = 1.f;
	float Qatt = 1.f;
};

class ILight
{
public:
	virtual ~ILight() = default;

	virtual void SetDiffuseColor(const dx::XMFLOAT3& diffuseColor)				noexcept = 0;
	virtual void SetDiffuseIntensity(float diffuseIntensity)					noexcept = 0;
	virtual void SetConstantAttenuation(const float& Catt)						noexcept = 0;
	virtual void SetLinearAttenuation(const float& Latt)						noexcept = 0;
	virtual void SetQuadAttenuation(const float& Qatt)							noexcept = 0;

	virtual LightDesc GetDesc() const noexcept = 0;

	// virtual dx::XMFLOAT3 GetDiffuseColor()						const noexcept = 0;
	// virtual float GetDiffuseIntensity()							const noexcept = 0;
	// virtual float GetConstantAttenuation()						const noexcept = 0;
	// virtual float GetLinearAttenuation()							const noexcept = 0;
	// virtual float GetQuadAttenuation()							const noexcept = 0;
};