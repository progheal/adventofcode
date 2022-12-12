#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
using namespace std;

vector<string> hmap;
int ex, ey;

struct State
{
	int x, y;
	bool partb;
	typedef int cost_t;
	bool isFinish() const
	{
		try
		{
			return partb ? hmap.at(x).at(y) == 'a' : (x == ex && y == ey);
		}
		catch(...)
		{
			return false;
		}
	}
	auto nextMoves() const
	{
		vector<pair<State, cost_t>> next;
		char me = hmap[x][y];
		int dir[5] = {0, 1, 0, -1, 0};
		for(int d = 0; d < 4; d++)
		{
			int nx = x + dir[d], ny = y + dir[d+1];
			try
			{
				char n = hmap.at(nx).at(ny);
				if(partb ? (n >= me-1) : (n <= me+1))
				{
					next.push_back({{nx, ny, partb}, 1});
				}
			}
			catch(...) {}
		}
		return next;
	}
	int id() const
	{
		return x * hmap[0].size() + y;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	hmap = AOC::readPicture(cin);
	int sx, sy;
	for(sx = 0; sx < hmap.size(); sx++)
	{
		if((sy = hmap[sx].find('S')) != string::npos)
		{
			hmap[sx][sy] = 'a';
			break;
		}
	}
	for(ex = 0; ex < hmap.size(); ex++)
	{
		if((ey = hmap[ex].find('E')) != string::npos)
		{
			hmap[ex][ey] = 'z';
			break;
		}
	}

	if(!partb)
		cout << AOC::BFS(State{sx, sy, false}) << endl;
	else
		cout << AOC::BFS(State{ex, ey, true}) << endl;

	return 0;
}

