#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

template<class G>
int simulate(const AOC::Grid<G>& g, const AOC::Coord& begin, const AOC::Vector& bdir)
{
	map<AOC::Coord, int, AOC::CoordOrder> beamexit;
	vector<pair<AOC::Coord, AOC::Vector>> beams, nextbeams;
	beams.push_back({begin, bdir});
	while(!beams.empty())
	{
		auto addBeam = [&](const AOC::Coord& pos, const AOC::Vector& dir)
		{
			int dirNum = (dir.x * 3 + dir.y + 3) / 2;
			int dirMask = 1 << dirNum;
			if((beamexit[pos] & dirMask) == 0)
			{
				beamexit[pos] |= dirMask;
				nextbeams.push_back({pos, dir});
			}
		};
		for(const auto& [pos, dir] : beams)
		{
			auto nextPos = pos + dir;
			if(!g.inBound(nextPos)) continue;
			switch(g[nextPos])
			{
			case '.':
				addBeam(nextPos, dir);
				break;
			case '-':
				if(dir.x == 0)
					addBeam(nextPos, dir);
				else
				{
					addBeam(nextPos, AOC::Vector::EAST);
					addBeam(nextPos, AOC::Vector::WEST);
				}
				break;
			case '|':
				if(dir.y == 0)
					addBeam(nextPos, dir);
				else
				{
					addBeam(nextPos, AOC::Vector::NORTH);
					addBeam(nextPos, AOC::Vector::SOUTH);
				}
				break;
			case '/':
				addBeam(nextPos, {-dir.y, -dir.x});
				break;
			case '\\':
				addBeam(nextPos, {dir.y, dir.x});
				break;
			}
		}
		beams.swap(nextbeams);
		nextbeams.clear();
	}
	return beamexit.size();
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> area = AOC::readPicture(cin);
	AOC::Grid g (area);

	if(!partb)
	{
		cout << simulate(g, {0, -1}, {0, 1}) << endl;
	}
	else
	{
		int max = 0;
		int w = g.width(), h = g.height();
		// East/West edge
		for(int i = 0; i < h; i++)
		{
			int west = simulate(g, {i, -1}, AOC::Vector::EAST);
			int east = simulate(g, {i, w+1}, AOC::Vector::WEST);
			max = std::max({max, west, east});
		}
		// North/South edge
		for(int j = 0; j < w; j++)
		{
			int north = simulate(g, {-1, j}, AOC::Vector::SOUTH);
			int south = simulate(g, {h+1, j}, AOC::Vector::NORTH);
			max = std::max({max, north, south});
		}
		cout << max << endl;
	}

	return 0;
}

