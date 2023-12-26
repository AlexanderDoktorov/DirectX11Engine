#pragma once
#include "Light.h"
#include "Model.h"

class Spotlight : public Light, public Model
{
	Spotlight(Graphics& Gfx) 
		: 
		Light(Gfx, LIGHT_TYPE_SPOTLIGHT),
		Model(Gfx, R"(.\Models\flashlight)", 
			aiProcess_CalcTangentSpace |
			aiProcess_GenNormals |
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded
		)
	{}
};