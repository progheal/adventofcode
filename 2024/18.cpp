#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

unordered_set<AOC::Coord> corrupted;
const int height = 71, width = 71;

struct State
{
	AOC::Coord c;
	State(const AOC::Coord& c) : c(c) {}
	typedef AOC::Path<AOC::Coord> cost_t;
	bool isFinish() const {return c.x == height - 1 && c.y == width - 1;}
	auto id() const {return c.hash();}
	auto nextMoves() const
	{
		vector<pair<State, cost_t>> ret;
		for(auto v : AOC::Vector::VonNeumannNeighborhood)
		{
			auto nc = c + v;
			if(nc.x < 0 || nc.x >= height || nc.y < 0 || nc.y >= width)
				continue;
			if(corrupted.count(nc) > 0) continue;
			ret.push_back({nc, cost_t{nc}});
		}
		return ret;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	AOC::Path<AOC::Coord> path;
	for(int i = 0; ; i++)
	{
		char c;
		int x, y;
		cin >> x >> c >> y;
		AOC::Coord cc{x, y};
		corrupted.insert(cc);
		if(i < 1024) continue;
		if(i == 1024)
		{
			path = AOC::BFS(State{AOC::Coord{0,0}});
			if(!partb)
			{
				cout << (int)path << "\n";
				return 0;
			}
			continue;
		}
		if(find(path.begin(), path.end(), cc) != path.end())
		{
			cerr << "Byte " << i << " " << cc << " blocked, find another route\n";
			path = AOC::BFS(State{AOC::Coord{0,0}});
			if(path.size() == 0)
			{
				cout << cc << "\n";
				return 0;
			}
		}
	}

	return 0;
}

