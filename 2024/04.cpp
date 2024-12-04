#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	auto v = AOC::readPicture(cin);
	AOC::Grid g (v, '.');

	int count = 0;
	if(!partb)
	{
		for(size_t x = 0; x < g.height(); x++)
		for(size_t y = 0; y < g.width(); y++)
		{
			AOC::Coord p {(int)x, (int)y};
			for(auto& dir : AOC::Vector::MooreNeighborhood)
			{
				if(g[p] == 'X' && g[p+dir] == 'M' && g[p+dir*2] == 'A' && g[p+dir*3] == 'S')
					++count;
			}
		}
	}
	else
	{
		for(size_t x = 0; x < g.height(); x++)
		for(size_t y = 0; y < g.width(); y++)
		{
			AOC::Coord p {(int)x, (int)y};
			string s;
			s += g[p+AOC::Vector::NORTHWEST];
			s += g[p+AOC::Vector::SOUTHEAST];
			s += g[p];
			s += g[p+AOC::Vector::NORTHEAST];
			s += g[p+AOC::Vector::SOUTHWEST];
			if(s == "MSAMS" || s == "MSASM" || s == "SMAMS" || s == "SMASM")
				++count;
		}
	}
	cout << count << "\n";

	return 0;
}

