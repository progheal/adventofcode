#pragma once

#include <format>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include "aoc/grid.h"

namespace AOC
{

/**
 * @brief 輸出為 XPM 圖片格式
 * @param out 輸出串流
 * @param grid 
 * @param color 像素顏色，以 { {"像素", "#RRGGBB"}...} 表示
 * @param conv 將元素轉換為像素表示
 */
template<class Cont, class Conv>
requires requires (Conv conv)
{
    { conv(typename AOC::Grid<Cont>::Element_t{}) } -> std::same_as<std::string>;
}
void xpm_export(
    std::ostream& out,
    const AOC::Grid<Cont>& grid,
    const std::map<std::string, std::string>& color,
    Conv conv)
{
    out << "/* XPM */\n"
        << "static char * xpm[] = {\n"
        << std::format("\"{} {} {} {}\",\n",
            grid.width(), grid.height(), color.size(), color.begin()->first.length());
    for(auto& [pixel, clr] : color)
    {
        out << std::format("\"{} c {}\",\n", pixel, clr);
    }
    for(auto p = grid.begin(); p != grid.end(); p++)
    {
        if(p.position().y == 0)
        {
            out << "\"";
        }
        out << conv(*p);
        if(p.position().y == grid.width() - 1)
        {
            out << "\",\n";
        }
    }
    out << "};\n";
}

/**
 * @brief 輸出為 XPM 圖片格式
 * @param out 輸出串流
 * @param grid 
 * @param color 像素顏色，以 { {"像素", "#RRGGBB"}...} 表示
 */
template<class Cont>
void xpm_export(
    std::ostream& out,
    const AOC::Grid<Cont>& grid,
    const std::map<std::string, std::string>& color)
{
    return xpm_export(out, grid, color,
        [](typename AOC::Grid<Cont>::Element_t c){
            using std::to_string;
            if constexpr(std::is_same_v<decltype(c), char>)
                return std::string(1, c);
            else
                return to_string(c);
        });
}

}