#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "IBindable.h"
#include "IGetID.h"

template<class T, typename... Args>
concept Resolvable = std::is_base_of_v<IBindable, T> && 
requires(Args... args)
{
	{ T::GenerateID(args...) } -> std::convertible_to<std::string>;
};

class BindableSystem
{
public:
	template<class T, class... Args> requires Resolvable<T, Args...>
	static std::shared_ptr<T> Resolve(Graphics& Gfx, Args&&... args)
	{
		return Get().ResolveImpl<T>(Gfx, std::forward<Args>(args)...);
	}
private:
	static BindableSystem& Get()
	{
		static BindableSystem bindSys{};
		return bindSys;
	}

	template<class T, class... Args>
	std::shared_ptr<T> ResolveImpl(Graphics& Gfx, Args&&... args)
	{
		// Recieve key for such parameters we want to create bindable with
		const std::string Key = T::GenerateID(std::forward<Args>(args)...);

		const auto i = bindableMap.find(Key);
		if( i == bindableMap.end() )
		{
			std::shared_ptr<T> bind = std::make_shared<T>(Gfx,std::forward<Args>(args)...);
			bindableMap[Key] = bind;
			return bind;
		}
		else
		{
			return std::static_pointer_cast<T>( i->second );
		}
	}
	std::unordered_map<std::string, std::shared_ptr<IBindable>> bindableMap;
};