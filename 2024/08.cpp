#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <numeric>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	auto v = AOC::readPicture(cin);
	AOC::Grid g (v, 0);

	map<char, vector<AOC::Coord>> antenna;
	for(auto p = g.begin(); p != g.end(); p++)
	{
		if(*p != '.') antenna[*p].push_back((AOC::Coord)p);
	}

	unordered_set<AOC::Coord> anti;
	auto genanti = [&](const AOC::Coord& c1, const AOC::Coord& c2)
	{
		if(!partb)
		{
			auto a = c1 + (c1 - c2);
			if(g.inBound(a)) anti.insert(a);
		}
		else
		{
			auto vec = c2 - c1;
			int G = gcd(vec.x, vec.y);
			auto unit = AOC::Vector{vec.x / G, vec.y / G};
			for(auto pos = c1; g.inBound(pos); pos += unit)
			{
				anti.insert(pos);
			}
		}
	};

	for(auto& [ant, vc] : antenna)
	{
		for(size_t i = 0; i < vc.size(); i++)
		for(size_t j = i+1; j < vc.size(); j++)
		{
			genanti(vc[i], vc[j]);
			genanti(vc[j], vc[i]);
		}
	}
	cout << anti.size() << "\n";

	return 0;
}

