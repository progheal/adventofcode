#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

struct Global
{
	AOC::Grid<vector<string>>* g;
	auto operator [] (const AOC::Coord& p)
	{
		return (*g)[p];
	}
} G;

struct State
{
	AOC::Coord pos;
	State(const AOC::Coord& pos) : pos(pos) {}
	typedef AOC::Path<AOC::Coord> cost_t;
	bool isFinish() const {return G[pos] == 'E';}
	auto id() const {return pos.hash();}
	auto nextMoves() const
	{
		vector<pair<State, cost_t>> ret;
		for(auto v : AOC::Vector::VonNeumannNeighborhood)
		{
			auto np = pos + v;
			if(G[np] != '#')
				ret.push_back({State{np}, cost_t{np}});
		}
		return ret;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	auto v = AOC::readPicture(cin);
	AOC::Grid g(v, '#');
	G.g = &g;

	auto start = g.find_first_of('S');
	vector<AOC::Coord> path = AOC::BFS(State{start});
	path.insert(path.begin(), start);
	unordered_map<AOC::Coord, int> remain;
	int rr = path.size();
	for(auto& c : path)
	{
		remain[c] = rr;
		--rr;
	}

	int count = 0;
	map<int, int> tally;
	int cheatLimit = partb ? 20 : 2;
	for(auto& c : path)
	for(int x = -cheatLimit; x <= cheatLimit; x++)
	{
		int ylim = cheatLimit-abs(x);
		for(int y = -ylim; y <= ylim; y++)
		{
			AOC::Vector cv {x, y};
			int t = abs(x) + abs(y);
			if(g[c+cv] != '#')
			{
				int dif = remain[c] - remain[c+cv];
				if(dif > t)
				{
					tally[dif-t]++;
					if(dif >= 100+t)
						count++;
				}
			}
		}
	}
	for(auto& [t, c] : tally) cerr << c << " cheats saved " << t << " ps.\n";
	cout << count << "\n";

	return 0;
}

