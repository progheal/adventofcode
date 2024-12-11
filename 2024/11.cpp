#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "aoc/aoc.h"
#include "aoc/cache.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	auto futuresize = AOC::cache_recursive([](auto& self, uint64_t begin, int step) -> uint64_t
	{
		if(step == 0) return 1;
		if(begin == 0) return self(self, 1, step-1);
		string str = to_string(begin);
		if(str.size() % 2 == 0)
		{
			uint64_t left = stoull(str.substr(0, str.size() / 2)),
				right = stoull(str.substr(str.size() / 2));
			return self(self, left, step-1) + self(self, right, step-1);
		}
		else
			return self(self, begin*2024, step-1);
	});

	int STEPS = partb ? 75 : 25;
	uint64_t n;
	uint64_t count = 0;
	while(cin >> n)
	{
		count += futuresize(n, STEPS);
	}
	cout << count << "\n";

	return 0;
}

