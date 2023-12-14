#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

template<class T>
int64_t roll(AOC::Grid<T>& g, AOC::Vector direction)
{
	int64_t pressure = 0;
	int h = g.height(), w = g.width();
	bool forward = direction.x < 0 || direction.y < 0;
	for(int i = 0; i < h; i++)
	{
		int gi = forward ? i : h-i-1;
		for(int j = 0; j < w; j++)
		{
			int gj = forward ? j : w-j-1;
			AOC::Coord c {gi, gj};
			if(g[c] == 'O')
			{
				AOC::Coord k = c + direction;
				for(; g.inBound(k); k += direction)
					if(g[k] != '.') break;
				k -= direction;
				g[c] = '.';
				g[k] = 'O';
				pressure += h - k.x;
			}
		}
	}
	return pressure;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> field = AOC::readPicture(cin);
	AOC::Grid g = field;

	if(!partb)
	{
		cout << roll(g, AOC::Vector::NORTH) << endl;
	}
	else
	{
		vector<vector<string>> pastField;
		vector<int64_t> pastCycle;
		pastField.push_back(field);
		pastCycle.push_back(-1);
		int64_t cycleBegin = -1, cycleLen = -1;
		for(int cyc = 1; cycleLen == -1; cyc++)
		{
			roll(g, AOC::Vector::NORTH);
			roll(g, AOC::Vector::WEST);
			roll(g, AOC::Vector::SOUTH);
			int64_t pressure = roll(g, AOC::Vector::EAST);
			pastCycle.push_back(pressure);
			pastField.push_back(field);
			//cerr << g << pressure << "\n";
			//cerr << pressure << " ";
			for(int i = cyc-1; i > 0; i--)
			{
				if(pastCycle[i] == pressure && pastField[i] == field)
				{
					cycleBegin = i;
					cycleLen = cyc - i;
					break;
				}
			}
		}
		//cerr << "\nFound cycle, start at " << cycleBegin << ", length " << cycleLen << endl;
		int64_t target = 1'000'000'000;
		int64_t equiv = (target - cycleBegin) % cycleLen + cycleBegin;
		//cerr << "Equivalent position: " << equiv << endl;
		cout << pastCycle[equiv] << endl;
	}

	return 0;
}

