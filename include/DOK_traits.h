#pragma once
#include <type_traits>

// Define a helper template with a member type 'type'
template <typename T, typename A = void>
struct HasValueMember : std::false_type {};

// Specialization for the case where 'T::value' is well-formed
template <typename T>
struct HasValueMember<T, std::void_t<decltype(T::value)>> : std::true_type {};

struct shader_type_pixel {};
struct shader_type_vertex {};
struct shader_type_hull {};
struct shader_type_compute {};
struct shader_type_geometry {};

template<class T, class A = void>
struct has_type_using
{
	static constexpr bool value = false;
};

template<class T>
struct has_type_using<T, std::void_t<typename T::type>>
{
	static constexpr bool value = true;
};

template<class T, class U = void>
struct has_bool_operator
{
	static constexpr bool value = false;
};

template<class T>
struct has_bool_operator<T, std::void_t<decltype(static_cast<bool>(std::declval<T>()))>>
{
	static constexpr bool value = true;
};

template<class T>
constexpr bool has_bool_operator_v = has_bool_operator<T>::value;

template<class T>
constexpr bool has_type_using_v = has_type_using<T>::value;


