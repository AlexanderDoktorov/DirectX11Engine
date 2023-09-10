#pragma once
#include "Graphics.h"

class GraphicsChild
{
protected:
	virtual ~GraphicsChild() = default;
	ID3D11Device* GetDevice(Graphics& Gfx);
	ID3D11DeviceContext* GetContext(Graphics& Gfx);
};