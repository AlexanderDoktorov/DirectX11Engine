#pragma once
#include "IndexedTriangleList.h"
#include "Graphics.h"
#include "HPipelineElements.h"
#include <DirectXMath.h>
#include <vector>

struct ObjBuffDiffNorm
{
	dx::XMFLOAT3 diffuseColor				= {};
	bool isNormalMapEnabled					= true;
	alignas(16) bool isDiffuseMapEnabled	= true;
};

class Mesh : public Drawable
{
	friend class Model;
	friend class Node;
public:
	Mesh() = default;
	Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables);
};