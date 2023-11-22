#pragma once
#include "IndexedTriangleList.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/Scene.h"
#include "Graphics.h"
#include "HPipelineElements.h"
#include <DirectXMath.h>
#include <vector>

class Mesh : public Drawable
{
public:
	Mesh() = default;
	Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables);
};