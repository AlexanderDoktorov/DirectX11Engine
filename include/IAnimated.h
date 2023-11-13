#pragma once

class IAnimated
{
public:
	virtual ~IAnimated() = default;
	virtual void Update(float dt) noexcept = 0;
};