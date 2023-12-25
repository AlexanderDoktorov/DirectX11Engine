#pragma once
#include "IAnimated.h"
#include "Model.h"
#include "ILight.h"

class PointLight : public LightBase<PointLightDesc>, public IBindable, public IAnimated, public Model, public IMovable, public IToString
{
public:
	PointLight(Graphics& Gfx);
	
	void		Reset() noexcept override;
	void		Bind(Graphics& Gfx) noexcept override;

	// IMovable
	virtual void SetPosition(float _x, float _y, float _z) override;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

	// IToString
	const char* ToString() const noexcept override;

	// IAnimated
	void Update(float dt) noexcept override;
	
protected:
	const float droll = 1.f;
	const float dyaw = 1.f;
	const float dpitch = 1.f;
};
