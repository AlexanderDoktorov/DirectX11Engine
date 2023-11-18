#pragma once
#include <DirectXMath.h>

static constexpr unsigned int NUM_MATERIALS = 4;

class Material
{
public:
    struct MaterialDesc
    {
        DirectX::XMFLOAT3  ambient  {1.f,1.f,1.f};
        DirectX::XMFLOAT3  diffuse  {1.f,1.f,1.f};
        DirectX::XMFLOAT3  specular {1.f,1.f,1.f};
        float              shininess{1.f};
    };

    enum MaterialID
    {
        Unknown,
        Emerald,
        Jade,
        Obsidian,
        Pearl,
        Bronze,
        Copper,
        RubberWhite,
        RubberRed,
        RubberYellow,
        RubbeGreen,
    };

    static constexpr MaterialDesc GetMaterialByID(MaterialID id) noexcept
    {
        switch (id)
        {
        case Material::Unknown:
            return MaterialDescs[0];
            break;
        case Material::Emerald:
            return MaterialDescs[1];
            break;
        case Material::Jade:
            return MaterialDescs[2];
            break;
        case Material::Obsidian:
            return MaterialDescs[3];
            break;
        case Material::Pearl:
            break;
        case Material::Bronze:
            break;
        case Material::Copper:
            break;
        case Material::RubberWhite:
            break;
        case Material::RubberRed:
            break;
        case Material::RubberYellow:
            break;
        case Material::RubbeGreen:
            break;
        }
        return MaterialDescs[0];
    }

    static constexpr MaterialDesc MaterialDescs[NUM_MATERIALS] = 
    {
        { { 1.f,        1.f,        1.f         },  { 1.f,      1.f,    1.f      }, { 1.f,       1.f,       1.f         }, 1.0f }, // Unknown
        { { 0.0215f,    0.1745f,    0.0215f     },  { 0.54f,    0.89f,  0.63f    }, { 0.316228f, 0.316228f, 0.316228f   }, 0.6f }, // Emerald
        { { 0.135f,	    0.2225f,	0.1575f     },  { 0.54f,    0.89f,	0.63f    }, { 0.316228f, 0.316228f,	0.316228f   }, 0.1f }, // Jade
        { { 0.05375f,	0.05f,	    0.06625f    },  {0.18275f,  0.17f,	0.22525f }, { 0.332741f, 0.328634f,	0.346435f   }, 0.3f }, // Obsidian
    };
};

