#include "StringHelper.h"

std::string CutAfter(const std::string& str_to_cut, char char_after) noexcept
{
    return std::string(str_to_cut.substr(str_to_cut.find(char_after) + 1));
}

std::string CutBefore(const std::string& str_to_cut, char char_before, bool include_char_before) noexcept
{
    if (!include_char_before)
        return std::string(str_to_cut.begin(), str_to_cut.begin() + str_to_cut.find(char_before));
    return std::string(str_to_cut.begin(), str_to_cut.begin() + str_to_cut.find(char_before) + 1);//std::string(str_to_cut.substr(str_to_cut.find(char_after) + 1));
}

