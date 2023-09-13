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

class LightSource : public SolidBall, public IBindable
{
public:
	LightSource(Graphics& Gfx);

	void		Draw(Graphics& Gfx) override;
	void		Reset();
	void		Bind(Graphics& Gfx) noexcept override;

	void		ShowControlChildWindow();

	// IMovable
	virtual void SetPosition(float _x, float _y, float _z) override;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

	// IToString
	const char* ToString() const noexcept override;

private:
	static std::unique_ptr<LightConstantBuffer<LightDesc>> pLightBuffer;
	LightDesc lightDesc;
};