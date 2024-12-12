#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

struct Global
{
    AOC::Grid<vector<string>>* g;
	unordered_set<AOC::Coord> visited;
	unordered_set<AOC::Coord> region;
	int perimeter;
	int area;
	auto operator [] (const AOC::Coord& p)
	{
		return (*g)[p];
	}
} G;

class State
{
	AOC::Coord pos;
public:
	State(const AOC::Coord& p): pos(p) {}
	typedef int cost_t;
	auto id() const {return pos.hash();}
	auto isFinish() const {return false;}
	auto nextMoves() const
	{
		G.visited.insert(pos);
		G.region.insert(pos);
		++G.area;
		vector<pair<State, cost_t>> ret;
		for(auto v : AOC::Vector::VonNeumannNeighborhood)
		{
			if(G.g->inBound(pos+v))
			{
				if(G[pos+v] == G[pos])
					ret.push_back({pos+v, 1});
				else
					G.perimeter++;
			}
			else
			{
				G.perimeter++;
			}
		}
		return ret;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	auto v = AOC::readPicture(cin);
	AOC::Grid g(v, 0);
	G.g = &g;

	int64_t pricesum = 0;
	for(auto p = g.begin(); p != g.end(); p++)
	{
		if(G.visited.count(p.position()) == 0)
		{
			G.region.clear();
			G.perimeter = G.area = 0;
			AOC::BFS(State{p.position()});
			if(!partb)
			{
				pricesum += G.perimeter * G.area;
				// cerr << "Region " << g[p.position()] << " from " << p.position() << " A " << G.area << " P " << G.perimeter << "\n";
			}
			else
			{
				int corners = 0;
				for(auto pos : G.region)
				{
					char curr = g[pos];
					for(auto ns : {AOC::Vector::NORTH, AOC::Vector::SOUTH})
					for(auto ew : {AOC::Vector::EAST, AOC::Vector::WEST})
					{
						if(g[pos+ns] != curr && g[pos+ew] != curr)
							++corners;
						else if(g[pos+ns] == curr && g[pos+ew] == curr && g[pos+ns+ew] != curr)
							++corners;
					}
				}
				pricesum += corners * G.area;
				// cerr << "Region " << g[p.position()] << " from " << p.position() << " A " << G.area << " S " << corners << "\n";
			}
		}
	}
	cout << pricesum << "\n";

	return 0;
}

