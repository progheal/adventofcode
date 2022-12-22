#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <cctype>
#include "aoc/aoc.h"
#include "aoc/util.h"
using namespace std;

int gcd(int a, int b)
{
	if(b) while((a %= b) && (b %= a));
	return b ? b : a;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> field = AOC::readPicture(cin);

	string inst = field.back();
	field.pop_back();
	// Add one row + one column space
	int h = field.size(), w = 0;
	for(auto& s : field) w = max(w, (int)s.size());
	w++;
	for(auto& s : field) s.resize(w, ' ');

	int x = 0, y = field[0].find_first_not_of(' ');
	int dir[5] = {0, 1, 0, -1, 0};
	int d = 0;

	// Cube net can only have 4x3 or 5x2, so GCD of input size is cube size
	int cubesize = gcd(h-1, w-1);

	auto ordinarystep = [&](int x, int y, int d)
	{
		return make_tuple((x + dir[d] + h) % h, (y + dir[d+1] + w) % w, d);
	};

	auto turnLeft = [](int d){return (d+3)%4;};
	auto turnRight = [](int d){return (d+1)%4;};

	// mapping of where an outgoing move will go to
	map<tuple<int, int, int>, tuple<int, int, int>> cubeEdge;
	if(partb)
	{
		enum
		{
			EDGELEFT = -1,
			EDGESTRAIGHT = 0,
			EDGERIGHT = 1
		};
		vector<tuple<int, int, int>> unmatchedEdge;
		vector<int> unmatchedTurn = {EDGERIGHT};
		bool reverse = false;
		int ex = x, ey = y, ed = d;
		do
		{
			if(!reverse)
			{
				for(int i = 0; i < cubesize; i++)
				{
					unmatchedEdge.push_back({ex, ey, ed});
					//cerr << ex << ' ' << ey << ' ' << ed << endl;
					tie(ex, ey, ed) = ordinarystep(ex, ey, ed);
				}
			}
			else
			{
				for(int i = 0; i < cubesize; i++)
				{
					auto [rx, ry, rd] = unmatchedEdge.back();
					unmatchedEdge.pop_back();
					cubeEdge.insert(make_pair(
						make_tuple(rx, ry, turnLeft(rd)),
						make_tuple(ex, ey, turnRight(ed))
					));
					cubeEdge.insert(make_pair(
						make_tuple(ex, ey, turnLeft(ed)),
						make_tuple(rx, ry, turnRight(rd))
					));
					//cerr << rx << ' ' << ry << ' ' << turnLeft(rd) << " -> "
					//	 << ex << ' ' << ey << ' ' << turnRight(ed) << "; "
					//	 << ex << ' ' << ey << ' ' << turnLeft(ed) << " -> "
					//	 << rx << ' ' << ry << ' ' << turnRight(rd) << endl;
					tie(ex, ey, ed) = ordinarystep(ex, ey, ed);
				}
			}
			char now = field[ex][ey];
			auto [lx, ly, ld] = ordinarystep(ex, ey, turnLeft(ed));
			char myleft = field[lx][ly];
			int edgeTurn = EDGELEFT;
			if(myleft == ' ' && now == ' ')
			{
				edgeTurn = EDGERIGHT;
			}
			else if(myleft == ' ' && now != ' ')
			{
				edgeTurn = EDGESTRAIGHT;
			}
			//cerr << "EdgeTurn " << edgeTurn << endl;
			if(reverse)
			{
				// edgeTurn won't be EDGELEFT here
				int lastUnmatchedTurn = unmatchedTurn.back();
				//cerr << "   Match " << lastUnmatchedTurn << endl;
				// EDGESTRAIGHT + EDGERIGHT
				if(lastUnmatchedTurn + edgeTurn == 1)
				{
					unmatchedTurn.pop_back();
					if(unmatchedTurn.empty())
					{
						unmatchedTurn.push_back(edgeTurn);
						reverse = false;
					}
				}
				else // EDGERIGHT + EDGERIGHT
				{
					unmatchedTurn.back() = EDGESTRAIGHT;
					reverse = false;
				}
			}
			else
			{
				if(edgeTurn == EDGELEFT)
					reverse = true;
				else
					unmatchedTurn.push_back(edgeTurn);
			}
			if(edgeTurn == EDGERIGHT)
			{
				ed = turnRight(ed);
				tie(ex, ey, ignore) = ordinarystep(ex, ey, turnRight(ed));
			}
			else if(edgeTurn == EDGELEFT)
			{
				ed = turnLeft(ed);
				tie(ex, ey, ignore) = ordinarystep(ex, ey, ed);
			}
		} while(!(ex == x && ey == y && ed == d));
	}

	auto cubestep = [&](int x, int y, int d)
	{
		auto pe = cubeEdge.find(make_tuple(x, y, d));
		if(pe != cubeEdge.end())
			return pe->second;
		else
			return ordinarystep(x, y, d);
	};

	auto walk = [&](int x, int y, int d, int steps)
	{
		while(steps--)
		{
			int px = x, py = y, pd = d;
			if(partb)
				tie(x, y, d) = cubestep(x, y, d);
			else
			{
				do
				{
					tie(x, y, d) = ordinarystep(x, y, d);
				} while(field[x][y] == ' ');
			}
			if(field[x][y] == '#')
			{
				return make_tuple(px, py, pd);
			}
		}
		return make_tuple(x, y, d);
	};

	int steps = 0;
	for(char c : inst)
	{
		if(isdigit(c))
		{
			steps = steps * 10 + (c - '0');
			continue;
		}
		else
		{
			//cerr << x << " " << y << " " << d << " x" << steps;
			tie(x, y, d) = walk(x, y, d, steps);
			if(c == 'R')
				d = turnRight(d);
			else
				d = turnLeft(d);
			//cerr << " -> " << x << " " << y << " " << d << endl;
			steps = 0;
		}
	}
	//cerr << x << " " << y << " " << d << " x" << steps;
	tie(x, y, d) = walk(x, y, d, steps);
	//cerr << " -> " << x << " " << y << " " << d << endl;

	cout << (x+1) * 1000 + (y+1) * 4 + d << endl;

	return 0;
}

