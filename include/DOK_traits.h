#pragma once
#include <type_traits>

// Define a helper template with a member type 'type'
template <typename T, typename A = void>
struct HasValueMember : std::false_type {};

// Specialization for the case where 'T::value' is well-formed
template <typename T>
struct HasValueMember<T, std::void_t<decltype(T::value)>> : std::true_type {};