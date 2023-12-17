#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

template <bool Ultra>
class State
{
	AOC::Grid<>* g;
	AOC::Coord pos;
	AOC::Vector dir;
public:
	typedef int cost_t;
	State(AOC::Grid<>* g, const AOC::Coord& p, const AOC::Vector& v)
		: g(g), pos(p), dir(v) {}
	bool isFinish() const
	{
		return pos.x == g->height()-1 && pos.y == g->width()-1;
	}
	auto nextMoves() const
	{
		constexpr int minS = Ultra ? 4 : 1,
		              maxS = Ultra ? 10 : 3;
		vector<pair<State, int>> ret;
		for(auto nextv : AOC::Vector::VonNeumannNeighborhood)
		{
			if(nextv == dir || nextv == dir.Rotate180()) continue;
			int cost = 0;
			auto nextPos = pos;
			for(int s = 1; s <= maxS; s++)
			{
				nextPos += nextv;
				if(!g->inBound(nextPos)) break;
				cost += (*g)[nextPos] - '0';
				if(s < minS) continue;
				ret.push_back({State(g, nextPos, nextv), cost});
				//cerr << "   > " << nextPos << ", " << nextv << ", +" << cost << endl;
			}
		}
		return ret;
	}
	size_t id() const
	{
		return pos.hash() * 5 + dir.hash();
	}
	int estimateRemain() const {return 0;}
	/*
	void visit(int cost) const
	{
		cerr << "Visiting " << pos << ", " << dir << ", cost = " << cost << endl;
	}
	*/
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> map = AOC::readPicture(cin);
	AOC::Grid<> g(map);

	if(!partb)
	{
		State<false> init (&g, {0, 0}, {0, 0});
		cout << AOC::AStar(init) << endl;
	}
	else
	{
		State<true> init (&g, {0, 0}, {0, 0});
		cout << AOC::AStar(init) << endl;
	}

	return 0;
}

