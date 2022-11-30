#pragma once

#include <vector>
#include <regex>
#include <string_view>
#include <string>

namespace AOC
{

std::vector<std::string> tokenize(std::string_view source, std::regex delimiter);
std::vector<std::string> tokenize(std::string_view source, std::string delimiter);

}

