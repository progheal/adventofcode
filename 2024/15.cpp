#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	const map<char, AOC::Vector> dir = {
		{'^', AOC::Vector::NORTH},
		{'>', AOC::Vector::EAST},
		{'<', AOC::Vector::WEST},
		{'v', AOC::Vector::SOUTH}
	};

	auto v = AOC::readPicture(cin);
	string step;
	while(v.back() != "")
	{
		step = v.back() + step;
		v.pop_back();
	}
	v.pop_back();
	if(partb)
	{
		for(auto& row : v)
		{
			string newrow;
			for(char c : row)
			{
				switch(c)
				{
				case '#': newrow += "##"; break;
				case 'O': newrow += "[]"; break;
				case '.': newrow += ".."; break;
				case '@': newrow += "@."; break;
				}
			}
			row = newrow;
		}
	}
	AOC::Grid g (v, '#');

	auto canmove = [&](auto& self, const AOC::Coord& pos, const AOC::Vector& dir) -> bool
	{
		if(g[pos] == '#') return false;
		if(g[pos] == '.') return true;
		if(g[pos] == 'O') return self(self, pos + dir, dir);
		// g[pos] == '[' || ']'
		AOC::Coord left = g[pos] == '[' ? pos : (pos + AOC::Vector::WEST),
			right = g[pos] == ']' ? pos : (pos + AOC::Vector::EAST);
		if(dir == AOC::Vector::WEST)
			return self(self, left + dir, dir);
		else if(dir == AOC::Vector::EAST)
			return self(self, right + dir, dir);
		else
			return self(self, left + dir, dir) && self(self, right + dir, dir);
	};
	auto domove = [&](auto& self, const AOC::Coord& pos, const AOC::Vector& dir) -> void
	{
		if(g[pos] == '#')
		{
			cerr << "blocked at " << pos << "\n";
			exit(1);
		}
		if(g[pos] == '.') return;
		if(g[pos] == 'O') 
		{
			self(self, pos + dir, dir);
			g[pos + dir] = 'O';
			g[pos] = '.';
			return;
		}
		// g[pos] == '[' || ']'
		AOC::Coord left = g[pos] == '[' ? pos : (pos + AOC::Vector::WEST),
			right = g[pos] == ']' ? pos : (pos + AOC::Vector::EAST);
		if(dir == AOC::Vector::WEST)
		{
			self(self, left + dir, dir);
			g[left + dir] = '[';
			g[left] = ']';
			g[right] = '.';
		}
		else if(dir == AOC::Vector::EAST)
		{
			self(self, right + dir, dir);
			g[right + dir] = ']';
			g[right] = '[';
			g[left] = '.';
		}
		else
		{
			self(self, left + dir, dir);
			self(self, right + dir, dir);
			g[left + dir] = '[';
			g[left] = '.';
			g[right + dir] = ']';
			g[right] = '.';
		}
	};

	AOC::Coord robot = g.find_first_of('@');
	g[robot] = '.';
	for(char c : step)
	{
		auto vec = dir.at(c);
		if(canmove(canmove, robot+vec, vec))
		{
			domove(domove, robot+vec, vec);
			robot += vec;
		}
		/*
		cerr << "Trying move " << c << ", field:\n";
		g[robot] = '@';
		for(auto& s : v) cerr << s << "\n";
		g[robot] = '.';
		 */
	}
	// g[robot] = '@';
	// for(auto& s : v) cerr << s << "\n";
	// g[robot] = '.';

	int sum = 0;
	for(auto p = g.begin(); p != g.end(); p++)
	{
		if(*p == 'O' || *p == '[')
		{
			auto c = p.position();
			sum += c.x * 100 + c.y;
		}
	}
	cout << sum << "\n";

	return 0;
}

