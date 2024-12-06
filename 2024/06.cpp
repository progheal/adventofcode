#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <unordered_set>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	auto v = AOC::readPicture(cin);
	AOC::Grid g(v, '.');

	AOC::Mob m {g.find_first_of('^'), AOC::Vector::NORTH}, gm = m;

	if(!partb)
	{
		unordered_set<AOC::Coord> mark;
		while(g.inBound(m))
		{
			mark.insert(m.pos);
			if(g[m.ahead()] == '#')
				m.CW90();
			else
				m.move();
		}
		cout << mark.size() << "\n";
	}
	else
	{
		unordered_set<AOC::Coord> checked;
		checked.insert(gm.pos);
		int count = 0;
		while(g.inBound(m))
		{
			auto ahead = m.ahead();
			if(g[ahead] == '#')
			{
				m.CW90();
				continue;
			}
			// g[ahead] == '.'
			if(checked.count(ahead) == 0)
			{
				checked.insert(ahead);
				g[ahead] = '#';
				AOC::Mob tortoise = m, hare = m;
				do
				{
					if(g[tortoise.ahead()] == '#')
						tortoise.CW90();
					else
						tortoise.move();
					if(g[hare.ahead()] == '#')
						hare.CW90();
					else
						hare.move();
					if(g[hare.ahead()] == '#')
						hare.CW90();
					else
						hare.move();
				} while(g.inBound(hare) && tortoise != hare);
				if(g.inBound(hare))
					++count;
				g[ahead] = '.';
			}
			m.move();
		}
		cout << count << "\n";
	}

	return 0;
}

