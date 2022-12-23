#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> field = AOC::readPicture(cin);
	vector<AOC::Coord> elves;
	map<int, map<int, int>> dmap;
	for(int i = 0; i < field.size(); i++)
		for(int j = 0; j < field[i].size(); j++)
			if(field[i][j] == '#')
			{
				elves.push_back({i, j});
				dmap[i][j] = 1;
				//cerr << "(" << i << "," << j << ")";
			}
	//cerr << endl;

	auto M = [&](const AOC::Coord& c)->int& {return dmap[c.x][c.y];};

	vector<array<AOC::Vector, 3>> priority = {
		{AOC::Vector::NORTH, AOC::Vector::NORTHWEST, AOC::Vector::NORTHEAST},
		{AOC::Vector::SOUTH, AOC::Vector::SOUTHWEST, AOC::Vector::SOUTHEAST},
		{AOC::Vector::WEST, AOC::Vector::NORTHWEST, AOC::Vector::SOUTHWEST},
		{AOC::Vector::EAST, AOC::Vector::NORTHEAST, AOC::Vector::SOUTHEAST}
	};

	int stepcount = 0;
	int elfstops;
	do
	{
		elfstops = 0;
		map<AOC::Coord, vector<int>, AOC::CoordOrder> propose;
		for(int i = 0; i < elves.size(); i++)
		{
			auto checkNeighbor = [&](const AOC::Vector& v){
						return M(elves[i] + v) == 0;
					};
			if(all_of(begin(AOC::Vector::MooreNeighborhood),
					  end(AOC::Vector::MooreNeighborhood),
					  checkNeighbor
					))
			{
				//cerr << elves[i] << " stops." << endl;
				elfstops++;
				continue;
			}
			for(int p = 0; p < 4; p++)
			{
				if(all_of(priority[p].begin(), priority[p].end(), checkNeighbor))
				{
					auto np = elves[i] + priority[p][0];
					propose[np].push_back(i);
					//cerr << elves[i] << " says " << np << endl;
					break;
				}
			}
		}
		for(auto& [np, v] : propose)
		{
			if(v.size() == 1)
			{
				int ei = v[0];
				M(elves[ei]) = 0;
				//cerr << ">> " << elves[ei] << " moves to " << np << endl;
				elves[ei] = np;
				M(np) = 1;
			}
		}
		priority.push_back(priority[0]);
		priority.erase(priority.begin());
		++stepcount;
		if(!partb && stepcount == 10) break;
		//for(auto c : elves) cerr << c;
		//cerr << endl;
	} while(elfstops < elves.size());

	if(!partb)
	{
		int xm = 0, xM = 0, ym = 0, yM = 0;
		for(auto c : elves)
		{
			xm = min(xm, c.x);
			xM = max(xM, c.x);
			ym = min(ym, c.y);
			yM = max(yM, c.y);
		}
		cout << (yM - ym + 1) * (xM - xm + 1) - elves.size() << endl;
	}
	else
	{
		cout << stepcount << endl;
	}

	return 0;
}

