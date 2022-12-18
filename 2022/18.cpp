#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int8_t world[30][30][30] = {{{}}};

	vector<vector<int>> lavas = AOC::readNumberMatrix(cin);
	for(auto c : lavas)
	{
		world[c[0]+1][c[1]+1][c[2]+1] = 1;
	}
	if(!partb)
	{
		int faces = 0;
		for(auto c : lavas)
		{
			int expose = 6;
			auto [x, y, z] = AOC::takeFirst<3>(c);
			++x, ++y, ++z;
			if(world[x-1][y][z]) --expose;
			if(world[x+1][y][z]) --expose;
			if(world[x][y-1][z]) --expose;
			if(world[x][y+1][z]) --expose;
			if(world[x][y][z-1]) --expose;
			if(world[x][y][z+1]) --expose;
			faces += expose;
		}
		cout << faces << endl;
	}
	else
	{
		int expose = 0;
		queue<tuple<int, int, int>> fill;
		fill.push({0,0,0});
		while(!fill.empty())
		{
			auto [x, y, z] = fill.front();
			fill.pop();
			if(world[x][y][z] != 0) continue;
			world[x][y][z] = 2;
			if(x > 0)  {if(world[x-1][y][z] == 1) ++expose; fill.push({x-1, y, z});}
			if(x < 29) {if(world[x+1][y][z] == 1) ++expose; fill.push({x+1, y, z});}
			if(y > 0)  {if(world[x][y-1][z] == 1) ++expose; fill.push({x, y-1, z});}
			if(y < 29) {if(world[x][y+1][z] == 1) ++expose; fill.push({x, y+1, z});}
			if(z > 0)  {if(world[x][y][z-1] == 1) ++expose; fill.push({x, y, z-1});}
			if(z < 29) {if(world[x][y][z+1] == 1) ++expose; fill.push({x, y, z+1});}
		}
		cout << expose << endl;
	}

	return 0;
}

