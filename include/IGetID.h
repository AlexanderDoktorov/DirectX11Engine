#pragma once
#include <typeinfo>

struct IGetID
{
	virtual ~IGetID() = default;
	virtual std::string GetID() const noexcept = 0;
};