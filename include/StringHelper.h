#pragma once

#include <string>

std::string CutAfter(const std::string& str_to_cut, char char_after) noexcept;
std::string CutBefore(const std::string& str_to_cut, char char_before, bool include_char_before = true) noexcept;