#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

int MAXSTEP;
AOC::Coord mid;

vector<int64_t> evenAndHalfPU;
int evenCount, oddCount;

template<class T>
class State
{
	AOC::Grid<T> *g;
	AOC::Coord pos;
	int steps;
public:

	State(AOC::Grid<T>* g, AOC::Coord start, int s = 0) : g(g), pos(start), steps(s) {}
	typedef int cost_t;
	bool isFinish() const {return false;}
	auto id() const {return pos.hash();}
	auto nextMoves() const
	{
		int h = g->height();
		int w = g->width();
		vector<pair<State, int>> ret;
		if(steps == MAXSTEP) return ret;
		for(auto& dir : AOC::Vector::VonNeumannNeighborhood)
		{
			auto np = pos + dir;
			auto rp = np;
			rp.x = (rp.x % h + h) % h;
			rp.y = (rp.y % w + w) % w;
			if((*g)[rp] != '#')
			{
				ret.push_back({State{g, np, steps+1}, 1});
			}
		}
		return ret;
	}
	void visit(int cost) const
	{
		static int next = -1;
		if(next != steps)
		{
			if(steps % (131*2) == 66)
			{
				cerr << "Count for 131*2*" << (steps/(131*2)) << "+65 = " << oddCount << endl;
				evenAndHalfPU.push_back(oddCount);
			}
			next = steps;
		}
		if(steps % 2) oddCount++; else evenCount++;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	const int PUCOUNT = 6;
	vector<string> map = AOC::readPicture(cin);
	AOC::Grid g (map);
	int h = g.height(), w = g.width();

	mid = g.find_first_of('S');
	MAXSTEP = h * PUCOUNT + 66;
	State s (&g, mid, 0);

	AOC::BFS(s);

	int ActualPUCount = 26501365 / h; // 202300

	int pu = 4;
	int64_t a, b, c, d;
	a = evenAndHalfPU[0];
	b = evenAndHalfPU[1];
	c = evenAndHalfPU[2];
	while(pu < ActualPUCount)
	{
		pu += 2;
		d = 3*(c - b) + a;
		a = b;
		b = c;
		c = d;
		//cerr << "Count for 131*2*" << (pu/2) << "+65 = " << d << endl;
	}
	cout << d << endl;

	return 0;
}

