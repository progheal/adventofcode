#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/grid.h"
#include "aoc/util.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	const int Xmin = -50, Xmax = 50, Ymin = -51, Ymax = 51;
	//const int Xmin = -5, Xmax = 5, Ymin = -3, Ymax = 3;
	vector<AOC::Mob> robots;
	for(auto line : AOC::LineReader{cin})
	{
		auto c = AOC::readNumbers<int>(line, true);
		robots.push_back({AOC::Coord{c[0] + Xmin, c[1] + Ymin}, AOC::Vector{c[2], c[3]}});
	}
	if(!partb)
	{
		int quadrant[4] = {0, 0, 0, 0};
		for(auto& r : robots)
		{
			//cout << r << " -> ";
			r.move(100);
			r.wrap(Xmin, Xmax, Ymin, Ymax);
			//cout << r << "\n";
			if(r.pos.x > 0 && r.pos.y > 0)
				quadrant[0]++;
			if(r.pos.x > 0 && r.pos.y < 0)
				quadrant[1]++;
			if(r.pos.x < 0 && r.pos.y < 0)
				quadrant[2]++;
			if(r.pos.x < 0 && r.pos.y > 0)
				quadrant[3]++;
		}
		cout << quadrant[0] * quadrant[1] * quadrant[2] * quadrant[3] << "\n";
	}
	else
	{
		uint64_t minsum = -1;
		int time = -1;
		int BOTS = robots.size();
		vector<AOC::Mob> snapshot;
		for(int i = 0; i < 101*103; i++)
		{
			// cerr << "Running " << (i+1) << "/" << (101*103) << "...\r";
			vector<int> x, y;
			for(auto& r : robots)
			{
				r.move(1);
				r.wrap(Xmin, Xmax, Ymin, Ymax);
				x.push_back(r.pos.x);
				y.push_back(r.pos.y);
			}
			sort(x.begin(), x.end());
			sort(y.begin(), y.end());
			int distsum = 0;
			for(int i = 0; i < BOTS; i++)
			{
				distsum += (x[i] + y[i]) * (2 * i - BOTS + 1);
			}
			if(minsum > distsum)
			{
				minsum = distsum;
				time = i;
				snapshot = robots;
			}
		}
		cerr << "\n";
		cout << "Min distance at " << (time+1) << ", where robot is like: \n";
		array<string, 103> pattern;
		for(int j = 0; j < 103; j++) pattern[j] = string(101, '.');
		for(auto& r : snapshot)
		{
			pattern[r.pos.y - Ymin][r.pos.x - Xmin] = '*';
		}
		for(auto& s : pattern) cout << s << "\n";
	}

	return 0;
}

