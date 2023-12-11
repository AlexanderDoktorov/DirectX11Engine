#pragma once
#include <optional>
#include "DOK_traits.h"
#include "MaterialProperty.h"
#include "noxnd.h"
#include <DirectXMath.h>

class MaterialPropertiesDesc
{
public:
	MaterialPropertiesDesc() = default;

	void AddProperty(const MaterialProperty& property);
	std::optional<MaterialProperty> GetProperty(std::string pKey, [[maybe_unused]] unsigned int = 0, [[maybe_unused]] unsigned int = 0) const noexcept;

	// return true if any properties has been changed
	bool ShowGUI(const char* label) noexcept;

	template<class ...Types>
	void EmplaceProperty(Types&&... args)
	{
		properties.emplace_back(args...);
	}
	template<class T>
	std::optional<T> GetPropertyAs(std::string pKey, [[maybe_unused]] unsigned int = 0, [[maybe_unused]] unsigned int = 0) const noxnd
	{
		static_assert(HasValueMember<PMap<T>>::value, "Trying to get property type with unknown type");
		if constexpr (HasValueMember<PMap<T>>::value)
		{
			for (auto& p : properties)
			{
				if (p.GetKey() == pKey && PMap<T>::value == p.GetType() && p.GetSize() == sizeof(T))
				{
					return *reinterpret_cast<const T*>(p.GetData().data());
				}
			}
		}
		assert(false && "Property was not found");
		return std::nullopt;
	}
	template<>
	std::optional<DirectX::XMFLOAT3> GetPropertyAs(std::string pKey, [[maybe_unused]] unsigned int, [[maybe_unused]] unsigned int) const noxnd
	{
		for (auto& p : properties)
		{
			if (p.GetKey() == pKey && aiPTI_Float == p.GetType() && p.GetSize() == sizeof(DirectX::XMFLOAT3))
			{
				return *reinterpret_cast<const DirectX::XMFLOAT3*>(p.GetData().data());
			}
		}
		assert(false && "Property was not found");
		return std::nullopt;
	}
private:
	std::vector<MaterialProperty> properties{};
	size_t selectedPropertyIndex = 0;
};