#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/bfs.h"
#include "aoc/grid.h"
using namespace std;

template<class T>
struct State
{
	AOC::Grid<T> *g;
	AOC::Coord p;
	int t;
	int goalX;
public:
	typedef int cost_t;
	State(AOC::Grid<T>* g, AOC::Coord p, int t, int gx): g(g), p(p), t(t), goalX(gx) {}
	bool isFinish() const
	{
		return p.x == goalX;
	}
	int id() const {return p.x*1000000+p.y*1000+t;}
	bool checkWind() const
	{
		std::tuple<AOC::Vector, char, size_t> check[4] = {
			{AOC::Vector::NORTH, 'v', g->height() - 2},
			{AOC::Vector::SOUTH, '^', g->height() - 2},
			{AOC::Vector::WEST,  '>', g->width() - 2},
			{AOC::Vector::EAST,  '<', g->width() - 2}
		};
		for(auto [d, c, lim] : check)
		{
			int dist = t % lim;
			auto windsrc = p + dist * d;
			if(!g->inBound(windsrc)) windsrc -= lim * d;
			if((*g)[windsrc] == c)
				return true;
		}
		return false;
	}
	auto nextMoves() const
	{
		vector<pair<State, int>> next;
		for(auto d : AOC::Vector::VonNeumannNeighborhood)
		{
			State ns = *this;
			ns.p += d;
			ns.t++;
			if((*g)[ns.p] != '#' && !ns.checkWind()) next.push_back({ns, 1});
		}
		{
			State ns = *this;
			ns.t++;
			if(!ns.checkWind()) next.push_back({ns, 1});
		}
		return next;
	}
	/*
	void visit(int cost) const
	{
		cerr << p << " " << t << endl;
	}
	*/
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> field = AOC::readPicture(cin);
	AOC::Grid g (field, '#');

	int sx = 0;
	int sy = field[0].find_first_not_of('#');
	int ex = g.height()-1;
	int ey = field[ex].find_first_not_of('#');

	State s (&g, AOC::Coord(sx, sy), 0, ex);
	int t = AOC::BFS(s);
	if(!partb)
	{
		cout << t << endl;
	}
	else
	{
		State s2 (&g, AOC::Coord(ex, ey), t, sx);
		int t2 = t+AOC::BFS(s2);
		State s3 (&g, AOC::Coord(sx, sy), t2, ex);
		int t3 = t2+AOC::BFS(s3);
		cout << t3 << endl;
	}

	return 0;
}

