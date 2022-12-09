#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "aoc/aoc.h"
using namespace std;

void tailMove(pair<int, int> head, pair<int, int> &tail)
{
	if(abs(head.first - tail.first) <= 1 && abs(head.second - tail.second) <= 1) return;
	if(tail.first > head.first) tail.first--;
	else if(tail.first < head.first) tail.first++;
	if(tail.second > head.second) tail.second--;
	else if(tail.second < head.second) tail.second++; 
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	set<pair<int, int>> p;
	p.insert({0, 0});
	int ropesize = 2;
	if(partb) ropesize = 10;
	vector<pair<int, int>> rope (ropesize, {0, 0});

	AOC::readPerLineTokenized(cin,
		[&](const vector<string>& token)
		{
			int dx = 0, dy = 0;
			switch(token[0][0])
			{
				case 'L': dx = 1; break;
				case 'R': dx = -1; break;
				case 'U': dy = 1; break;
				case 'D': dy = -1; break;
			}
			int count = stoi(token[1]);
			while(count--)
			{
				rope[0].first += dx;
				rope[0].second += dy;
				for(int i = 1; i < ropesize; i++)
				{
					tailMove(rope[i-1], rope[i]);
				}
				p.insert(rope[ropesize-1]);
			}
		});
	cout << p.size() << endl;

	return 0;
}

