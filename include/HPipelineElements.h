#pragma once

#pragma region Buffers

	#include "VertexConstantBuffer.h"
	#include "PixelConstantBuffer.h"
	#include "IndexBuffer.h"
	#include "VertexBuffer.h"
	#include "TransformBuffer.h"
	#include "TransformBufferPS.h"
	#include "DataBufferPS.h"
	#include "ColorBuffer.h"

	#include "StructuredBuffer.h"
	#include "StructuredBufferVS.h"


#pragma endregion Buffers

#pragma region Shaders

	#include "PixelShader.h"
	#include "VertexShader.h"
	#include "VertexShaderCommon.h"
	#include "PixelShaderCommon.h"

#pragma endregion Shaders

#pragma region Textures

	#include "Sampler.h"
	#include "Texture2D.h"
	#include "RenderTexture.h"
	#include "ShaderResoursesPS.h"

#pragma endregion Textures

#include "InputLayout.h"
#include "Topology.h"

#define SLOT_TEXTURE_DIFFUSE 3
#define SLOT_TEXTURE_NORMALMAP 4
#define SLOT_TEXTURE_SPECULAR 5
#define SLOT_TEXTURE_HEIGHT 6

// ARRAYS //
#define SLOT_TEXTURE_ARRAY_DIFFUSE 0
#define SLOT_TEXTURE_ARRAY_NORMALMAP 1
#define SLOT_TEXTURE_ARRAY_SPECULAR 1
#define SLOT_TEXTURE_ARRAY_HEIGHT 2

#define SLOT_TRANSFORM_BUFFER 0
#define SLOT_MATERIAL_STRUCTURED_BUFFER 4
#define SLOT_SAMPLER 0
