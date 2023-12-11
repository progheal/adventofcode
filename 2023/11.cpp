#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> galaxy = AOC::readPicture(cin);
	vector<AOC::Coord> pos;
	map<int, int64_t> xMap, yMap;
	set<int> xC, yC;
	for(int i = 0; i < galaxy.size(); i++)
	{
		for(int j = 0; j < galaxy[i].size(); j++)
		{
			if(galaxy[i][j] == '#')
			{
				xC.insert(i);
				yC.insert(j);
				pos.push_back({i, j});
			}
		}
	}
	int64_t expandCoefficient = partb ? 1000000 : 2;
	int count = 0;
	for(int x : xC)
	{
		int skip = x - count;
		xMap[x] = x + skip * (expandCoefficient - 1);
		++count;
	}
	count = 0;
	for(int y : yC)
	{
		int skip = y - count;
		yMap[y] = y + skip * (expandCoefficient - 1);
		++count;
	}
	int64_t sum = 0;
	for(int i = 0; i < pos.size(); i++)
	{
		for(int j = i+1; j < pos.size(); j++)
		{
			sum += abs(xMap[pos[i].x] - xMap[pos[j].x]) +
			       abs(yMap[pos[i].y] - yMap[pos[j].y]);
		}
	}
	cout << sum << endl;

	return 0;
}

