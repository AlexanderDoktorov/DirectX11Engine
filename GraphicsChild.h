#pragma once
#include "Graphics.h"

class GraphicsChild
{
protected:
	ID3D11Device* GetDevice(Graphics& Gfx);
	ID3D11DeviceContext* GetContext(Graphics& Gfx);
};