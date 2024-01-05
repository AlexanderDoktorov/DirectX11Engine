#pragma once

#pragma once

#define t(slot) slot
#define b(slot) slot
#define s(slot) slot

// Textures //
#define SLOT_TEXTURE_DIFFUSE         t(0)
#define SLOT_TEXTURE_NORMALMAP       t(1)
#define SLOT_TEXTURE_SPECULAR        t(2)
#define SLOT_TEXTURE_HEIGHT          t(3)
#define SLOT_LIGHT_STRUCTURED_BUFFER t(4)

#define SLOT_SAMPLER_LINEAR	 s(0)

// Buffers //
#define SLOT_BUFFER_SCENE				b(0)
#define SLOT_TRANSFORM_BUFFER			b(0)
#define SLOT_BUFFER_MATERIAL			b(1)
#define SLOT_BUFFER_LIGHT				b(1)
#define SLOT_MATERIAL_STRUCTURED_BUFFER b(5)
