#include "Texture2D.h"

Texture2D::Texture2D(Graphics& Gfx, const std::string& type, const std::string& path)
	: PictureTexture(Gfx, path.c_str()), type(type), path(path)
{

}