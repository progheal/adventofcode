#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	map<int, set<int>> after;
	auto sorter = [&](int a, int b){return after[a].count(b) > 0;};
	int midsum = 0;
	AOC::readPerLine(cin, [&](const string& line)
	{
		if(line.find('|') != string::npos)
		{
			auto [u, v] = AOC::takeFirst<2, int>(AOC::readNumbers(line));
			after[u].insert(v);
		}
		else
		{
			auto v = AOC::readNumbers(line);
			if(v.size() % 2 == 0)
			{
				cerr << "Which mid on " << line << "???";
				return;
			}
			if(is_sorted(v.begin(), v.end(), sorter))
			{
				if(!partb)
					midsum += v[v.size() / 2];
			}
			else
			{
				if(partb)
				{
					sort(v.begin(), v.end(), sorter);
					midsum += v[v.size() / 2];
				}
			}
		}
	});
	cout << midsum << "\n";

	return 0;
}

