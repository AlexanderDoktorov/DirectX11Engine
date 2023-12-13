#pragma once
#include <string>
#include <typeinfo>

struct IGetID
{
	virtual ~IGetID() = default;
	virtual std::string GetID() const noexcept = 0;
};