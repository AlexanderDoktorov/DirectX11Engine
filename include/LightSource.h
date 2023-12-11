#pragma once
#include "imgui.h"
#include "PixelConstantBuffer.h"
#include "ILight.h"
#include "IAnimated.h"
#include "Model.h"

class LightSource : public IBindable, public ILight, public IAnimated, public Model, public IMovable, public IToString
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

	// IAnimated
	void Update(float dt)														noexcept override;

	// SolidBall
	virtual DirectX::XMMATRIX GetTransform()							  const noexcept override;

	virtual LightDesc GetDesc() const noexcept override;
private:

	const float droll = 1.f;
	const float dyaw = 1.f;
	const float dpitch = 1.f;

	static std::unique_ptr<PixelConstantBuffer<LightDesc>> pLightBuffer;
	LightDesc lightDesc;
};