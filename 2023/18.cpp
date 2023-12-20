#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	AOC::Coord pos;
	int64_t area2 = 0;
	int64_t peri = 0;
	AOC::readPerLine(cin, [&](const string& s)
	{
		vector<string> cmd = AOC::readToVector<string>(s);
		const AOC::Vector p2dir[] = {
			AOC::Vector::EAST,
			AOC::Vector::SOUTH,
			AOC::Vector::WEST,
			AOC::Vector::NORTH,
		};
		AOC::Vector dir;
		int64_t len;
		if(!partb)
		{
			switch(cmd[0][0])
			{
			case 'R': dir = AOC::Vector::EAST; break;
			case 'D': dir = AOC::Vector::SOUTH; break;
			case 'L': dir = AOC::Vector::WEST; break;
			case 'U': dir = AOC::Vector::NORTH; break;
			}
			len = stol(cmd[1]);
		}
		else
		{
			int hex = stol(cmd[2].substr(2, 6), nullptr, 16);
			len = hex / 16;
			dir = p2dir[hex % 16];
		}

		area2 += (pos.x * dir.y - pos.y * dir.x) * len;
		peri += len;
		pos += len * dir;
	});

	if(area2 < 0) area2 = -area2;
	cout << area2 / 2 + peri / 2 + 1 << endl;

	return 0;
}

