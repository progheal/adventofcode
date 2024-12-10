#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	auto v = AOC::readPicture(cin);
	AOC::Grid g(v);

	unordered_map<AOC::Coord, unordered_multiset<AOC::Coord>> trail;
	queue<AOC::Coord> q;
	for(auto p = g.begin(); p != g.end(); p++)
	{
		if(*p == '0')
		{
			q.push(p.position());
			trail[p.position()].insert(p.position());
		}
	}

	int pacount = 0, pbcount = 0;
	unordered_set<AOC::Coord> saved;
	while(!q.empty())
	{
		AOC::Coord curr = q.front();
		q.pop();
		if(g[curr] == '9')
		{
			pbcount += trail[curr].size();
			unordered_set<AOC::Coord> s {trail[curr].begin(), trail[curr].end()};
			pacount += s.size();
			continue;
		}
		for(auto v : AOC::Vector::VonNeumannNeighborhood)
		{
			auto next = curr+v;
			if(!g.inBound(next)) continue;
			if(g[next] != g[curr] + 1) continue;
			trail[next].insert(trail[curr].begin(), trail[curr].end());
			if(saved.count(next) > 0) continue;
			saved.insert(next);
			q.push(next);
		}
	}
	if(!partb)
		cout << pacount << "\n";
	else
		cout << pbcount << "\n";

	return 0;
}

