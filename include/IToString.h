#pragma once

class IToString
{
public:
	virtual ~IToString() = default;

	virtual const char* ToString() const noexcept = 0;
};