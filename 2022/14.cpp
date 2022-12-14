#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

char field[500][1000] = {};

bool addSand()
{
	int sx = 500, sy = 0;
	bool stopped = false;
	if(field[sy][sx] != 0) return false;
	do
	{
		if(sy == 500)
			stopped = true;
		else if(field[sy+1][sx] == 0)
			++sy;
		else if(field[sy+1][sx-1] == 0)
			++sy, --sx;
		else if(field[sy+1][sx+1] == 0)
			++sy, ++sx;
		else
			stopped = true;
	} while(!stopped);
	if(sy != 500) field[sy][sx] = 'o';
	return sy != 500;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int maxy = 0;
	AOC::readPerLineNumbers(cin,
		[&](const vector<int>& wall)
		{
			maxy = max(maxy, wall[1]);
			for(int i = 2; i < wall.size(); i += 2)
			{
				int px = wall[i-2], py = wall[i-1], cx = wall[i], cy = wall[i+1];
				maxy = max(maxy, cy);
				if(px == cx)
				{
					int m = min(py, cy), M = max(py, cy);
					for(int y = m; y <= M; y++) field[y][px] = '#';
				}
				else
				{
					int m = min(px, cx), M = max(px, cx);
					for(int x = m; x <= M; x++) field[py][x] = '#';
				}
			}
		});
	maxy+=2;
	if(partb)
	{
		for(int x = 0; x < 1000; x++) field[maxy][x] = '#';
	}
	int sands = 0;
	while(addSand()) ++sands;
	cout << sands << endl;

	return 0;
}

