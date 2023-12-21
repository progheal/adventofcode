#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

const int MAXSTEP = 64;

template<class T>
class State
{
	AOC::Grid<T> *g;
	AOC::Coord pos;
	int steps;
public:
	static vector<vector<int>>* dist;

	State(AOC::Grid<T>* g, AOC::Coord start, int s = 0) : g(g), pos(start), steps(s) {}
	typedef int cost_t;
	bool isFinish() const {return false;}
	auto id() const {return pos.hash();}
	auto nextMoves() const
	{
		vector<pair<State, int>> ret;
		if(steps == MAXSTEP) return ret;
		for(auto& dir : AOC::Vector::VonNeumannNeighborhood)
		{
			auto np = pos + dir;
			if(g->inBound(np) && (*g)[np] != '#')
			{
				ret.push_back({State{g, np, steps+1}, 1});
			}
		}
		return ret;
	}
	void visit(int cost) const
	{
		(*dist)[pos.x][pos.y] = steps;
	}
};

template<class T>
vector<vector<int>> * State<T>::dist = nullptr;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> map = AOC::readPicture(cin);
	AOC::Grid g (map);
	vector<vector<int>> dist(g.height(), vector<int>(g.width(), g.height() * g.width()));

	State s (&g, g.find_first_of('S'), 0);
	s.dist = &dist;

	AOC::BFS(s);
	int total = 0;
	for(auto& row : dist)
	{
		total += count_if(begin(row), end(row), [](int x){return x <= 64 && x % 2 == 0;});
	}
	cout << total << endl;

	return 0;
}

