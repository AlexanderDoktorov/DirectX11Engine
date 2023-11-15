#pragma once
#include "imgui.h"
#include "SolidBall.h"
#include "LightConstantBuffer.h"
#include "ILight.h"

class LightSource : public SolidBall, public IBindable, public ILight
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

	// ILight
	virtual void SetDiffuseColor(const dx::XMFLOAT3& diffuseColor)				noexcept override;
	virtual void SetDiffuseIntensity(float diffuseIntensity)					noexcept override;
	virtual void SetConstantAttenuation(const float& Catt)						noexcept override;
	virtual void SetLinearAttenuation(const float& Latt)						noexcept override;
	virtual void SetQuadAttenuation(const float& Qatt)							noexcept override;

	virtual LightDesc GetDesc() const noexcept override;

private:
	static std::unique_ptr<LightConstantBuffer<LightDesc>> pLightBuffer;
	LightDesc lightDesc;
};