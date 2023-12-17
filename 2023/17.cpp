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
	int straight;
public:
	typedef int cost_t;
	State(AOC::Grid<>* g, const AOC::Coord& p, const AOC::Vector& v, int s)
		: g(g), pos(p), dir(v), straight(s) {}
	bool isFinish() const
	{
		return pos.x == g->height()-1 && pos.y == g->width()-1;
	}
	auto nextMoves() const
	{
		vector<pair<State, int>> ret;
		for(auto nextv : AOC::Vector::VonNeumannNeighborhood)
		{
			if(nextv == dir.Rotate180()) continue;
			bool isStraight = nextv == dir;
			if constexpr (!Ultra)
			{
				if(isStraight && straight >= 3) continue;
			}
			else
			{
				if(isStraight && straight >= 10) continue;
			}
			auto nextPos = pos + nextv;
			if(!g->inBound(nextPos)) continue;
			int cost = (*g)[nextPos] - '0';
			int nextstraight = isStraight ? straight + 1 : 1;
			if constexpr (Ultra)
			{
				if(!isStraight)
				{
					if(!g->inBound(nextPos + 3 * nextv)) continue;
					cost += (*g)[nextPos + nextv] - '0';
					cost += (*g)[nextPos + 2 * nextv] - '0';
					cost += (*g)[nextPos + 3 * nextv] - '0';
					nextPos += 3 * nextv;
					nextstraight = 4;
				}
			}
			ret.push_back({State(g, nextPos, nextv, nextstraight), cost});
			//cerr << "   > " << nextPos << ", " << nextv << ", " << nextstraight << ", +" << cost << endl;
		}
		return ret;
	}
	size_t id() const
	{
		return (pos.hash() * 5 + dir.hash()) * 15 + straight;
	}
	int estimateRemain() const {return 0;}
	/*
	void visit(int cost) const
	{
		cerr << "Visiting " << pos << ", " << dir << ", " << straight << ", cost = " << cost << endl;
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
		State<false> init (&g, {0, 0}, {0, 0}, 0);
		cout << AOC::AStar(init) << endl;
	}
	else
	{
		State<true> init (&g, {0, 0}, {0, 0}, 0);
		cout << AOC::AStar(init) << endl;
	}

	return 0;
}

