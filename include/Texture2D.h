#pragma once
#include "PictureTexture.h"
#include <DirectXMath.h>
#include <string>

class Texture2D : public PictureTexture
{
public:
	Texture2D() = default;
	Texture2D(Graphics& Gfx, const std::string& type, const std::string& path);

	std::string type;
	std::string path;
};