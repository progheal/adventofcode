#ifdef DEBUG
#include <iostream>
#endif
#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>
#include "aoc/ocr.h"
using namespace std;

namespace AOC
{

// Source: https://github.com/mstksg/advent-of-code-ocr/blob/main/src/Advent/OCR/LetterMap.hs
static const char* Glyph4x6[6] = {
".##..###...##..####.####..##..#..#..###...##.#..#.#.....##..###..###...###.#..#.#...#####.",
"#..#.#..#.#..#.#....#....#..#.#..#...#.....#.#.#..#....#..#.#..#.#..#.#....#..#.#...#...#.",
"#..#.###..#....###..###..#....####...#.....#.##...#....#..#.#..#.#..#.#....#..#..#.#...#..",
"####.#..#.#....#....#....#.##.#..#...#.....#.#.#..#....#..#.###..###...##..#..#...#...#...",
"#..#.#..#.#..#.#....#....#..#.#..#...#..#..#.#.#..#....#..#.#....#.#.....#.#..#...#..#....",
"#..#.###...##..####.#.....###.#..#..###..##..#..#.####..##..#....#..#.###...##....#..####.",
};
static const char* Glyph4x6Map = "ABCEFGHIJKLOPRSUYZ";

static const char* Glyph6x10[10] = {
"..##...#####...####..######.######..####..#....#....###.#....#.#......#....#.#####..#####..#....#.######.",
".#..#..#....#.#....#.#......#......#....#.#....#.....#..#...#..#......##...#.#....#.#....#.#....#......#.",
"#....#.#....#.#......#......#......#......#....#.....#..#..#...#......##...#.#....#.#....#..#..#.......#.",
"#....#.#....#.#......#......#......#......#....#.....#..#.#....#......#.#..#.#....#.#....#..#..#......#..",
"#....#.#####..#......#####..#####..#......######.....#..##.....#......#.#..#.#####..#####....##......#...",
"######.#....#.#......#......#......#..###.#....#.....#..##.....#......#..#.#.#......#..#.....##.....#....",
"#....#.#....#.#......#......#......#....#.#....#.....#..#.#....#......#..#.#.#......#...#...#..#...#.....",
"#....#.#....#.#......#......#......#....#.#....#.#...#..#..#...#......#...##.#......#...#...#..#..#......",
"#....#.#....#.#....#.#......#......#...##.#....#.#...#..#...#..#......#...##.#......#....#.#....#.#......",
"#....#.#####...####..######.#.......###.#.#....#..###...#....#.######.#....#.#......#....#.#....#.######.",
};
static const char* Glyph6x10Map = "ABCEFGHJKLNPRXZ";

int difference(const string_view& s1, const string_view& s2, char bg, char fg)
{
	int d = 0;
	for(int i = 0; i < s1.size(); i++)
	{
		if(!((s1[i] == bg && s2[i] == '.') || (s1[i] == fg && s2[i] == '#')))
			++d;
	}
	return d;
}

string ocr(const vector<string>& glyph)
{
	int height = glyph.size();
	if(height != 6 && height != 10) return "";
	const char* charmap = height == 10 ? Glyph6x10Map : Glyph4x6Map;
	const char** board = height == 10 ? Glyph6x10 : Glyph4x6;
	int width = height == 10 ? 6 : 4;
	int tolerance = height == 10 ? 2 : 1;

	vector<string_view> glyphview;
	copy(glyph.begin(), glyph.end(), back_inserter(glyphview));

	char bgGuess = glyph[0][width];
	char fgGuess = glyph[0][glyph[0].find_first_not_of(bgGuess)];

	string ret;

	for(int inputW = 0; inputW < glyph[0].size(); inputW += width+1)
	{
#ifdef DEBUG
		for(int h = 0; h < height; h++) cerr << glyphview[h].substr(inputW, width) << endl;
#endif
		int index = -1;
		for(int i = 0; charmap[i] != 0; i++)
		{
			int dif = 0;
			for(int row = 0; row < height; row++)
			{
				dif += difference(glyphview[row].substr(inputW, width), string_view(&board[row][i*(width+1)], width), bgGuess, fgGuess);
			}
#ifdef DEBUG
			cerr << inputW << " " << charmap[i] << " " << dif << endl;
#endif
			if(dif <= tolerance)
			{
				index = i;
				break;
			}
		}
		if(index != -1)
		{
			ret += charmap[index];
		}
		else
		{
			ret += '?';
		}
	}

	return ret;
}


}
