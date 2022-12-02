#pragma once

#include <vector>
#include <regex>
#include <string_view>
#include <string>

namespace AOC
{

// 將字串以某字串或某 regex 切割
// aoc.h 的讀取函數可處理單一字元切割，此處則處理較複雜的切割點
std::vector<std::string> tokenize(std::string_view source, std::regex delimiter);
std::vector<std::string> tokenize(std::string_view source, std::string delimiter);

}

