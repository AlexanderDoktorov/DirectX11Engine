#pragma once
#include "IndexedTriangleList.h"

class IModel
{
public:
	virtual ~IModel() = default;
	virtual const IndexedTriangleList& GetModel() const noexcept = 0;
	virtual void SetModel() noexcept = 0;
};