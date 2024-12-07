#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int64_t total = 0;
	AOC::readPerLine(cin, [&](const string& line)
	{
		auto v = AOC::readNumbers<int64_t>(line);
		unordered_set<int64_t> before, after;
		after.insert(v[0]);
		for(size_t i = v.size()-1; i >= 2; i--)
		{
			if(!partb)
			{
				for(int64_t p : after)
				{
					if(p > v[i])
						before.insert(p-v[i]);
					if(p >= v[i] && p % v[i] == 0)
						before.insert(p/v[i]);
				}
			}
			else
			{
				for(int64_t p : after)
				{
					if(p > v[i])
						before.insert(p-v[i]);
					if(p >= v[i] && p % v[i] == 0)
						before.insert(p/v[i]);
					int64_t ten = 1;
					while(ten <= v[i]) ten *= 10;
					if(p >= ten && p % ten == v[i])
						before.insert(p / ten);
				}
			}
			after.swap(before);
			before.clear();
		}
		if(after.count(v[1]) > 0) total += v[0];
	});
	cout << total << "\n";

	return 0;
}

