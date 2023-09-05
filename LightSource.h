#pragma once
#include "imgui.h"
#include "SolidBall.h"
#include "LightConstantBuffer.h"
#include <array>

struct LightDesc
{
	alignas(16) dx::XMFLOAT3 pos = dx::XMFLOAT3();
	alignas(16) dx::XMFLOAT3 diffuseColor = dx::XMFLOAT3();
	float diffuseIntensity = 1.f;
	float Catt = 1.f;
	float Latt = 1.f;
	float Qatt = 1.f;
};

class LightSource : public IPlacableItem, public IToString
{
public:
	LightSource(Graphics& Gfx);

	void		Draw(Graphics& Gfx) noexcept;
	void		Reset();
	void		Bind(Graphics& Gfx);

	void ShowControlChildWindow();

	// IPlacableItem
	virtual void SetWorldPosition(const dx::XMFLOAT3& new_Wpos) override;
	virtual dx::XMFLOAT3 GetWorldPosition() const noexcept override;

	// IToString
	const char* ToString() const noexcept override;

private:
	static std::unique_ptr<LightConstantBuffer<LightDesc>> pLightBuffer;
	LightDesc lightDesc;
	SolidBall mesh;
};