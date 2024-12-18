#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

unordered_map<AOC::Coord, int> corrupted;
const int height = 71, width = 71;
int t;

struct State
{
	AOC::Coord c;
	State(const AOC::Coord& c) : c(c) {}
	typedef int cost_t;
	bool isFinish() const {return c.x == height - 1 && c.y == width - 1;}
	auto id() const {return c.hash();}
	auto nextMoves() const
	{
		vector<pair<State, int>> ret;
		for(auto v : AOC::Vector::VonNeumannNeighborhood)
		{
			auto nc = c + v;
			if(nc.x < 0 || nc.x >= height || nc.y < 0 || nc.y >= width)
				continue;
			auto p = corrupted.find(nc);
			if(p != corrupted.end() && p->second < t) continue;
			ret.push_back({nc, 1});
		}
		return ret;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	vector<AOC::Coord> idx;

	int i = 0;
	char c;
	int x, y;
	while(cin >> x >> c >> y)
	{
		AOC::Coord cc{x, y};
		corrupted[cc] = i;
		idx.push_back(cc);
		++i;
	}
	if(!partb)
	{
		t = 1024;
		int cost = AOC::BFS(State{AOC::Coord{0,0}});
		cout << cost << "\n";
		return 0;
	}
	int left = 0, right = corrupted.size();
	while(right - left > 1)
	{
		t = (left + right) / 2;
		int cost = AOC::BFS(State{AOC::Coord{0,0}});
		if(cost == 0)
			right = t;
		else
			left = t;
	}
	cout << idx[left] << "\n";

	return 0;
}

