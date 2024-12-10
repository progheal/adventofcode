#include <iostream>
#include <vector>
#include <string>
#include <queue>
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

	int pacount = 0, pbcount = 0;
	for(auto p = g.begin(); p != g.end(); p++)
	{
		if(*p != '0') continue;
		unordered_map<AOC::Coord, int> trail;
		queue<AOC::Coord> q;
		unordered_set<AOC::Coord> saved;
		q.push(p.position());
		trail[p.position()] = 1;
		while(!q.empty())
		{
			AOC::Coord curr = q.front();
			q.pop();
			if(g[curr] == '9')
			{
				++pacount;
				pbcount += trail[curr];
				continue;
			}
			for(auto v : AOC::Vector::VonNeumannNeighborhood)
			{
				auto next = curr+v;
				if(!g.inBound(next)) continue;
				if(g[next] != g[curr] + 1) continue;
				trail[next] += trail[curr];
				if(saved.count(next) > 0) continue;
				saved.insert(next);
				q.push(next);
			}
		}
	}
	if(!partb)
		cout << pacount << "\n";
	else
		cout << pbcount << "\n";

	return 0;
}

