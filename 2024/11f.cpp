#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	map<uint64_t, size_t> stones;
	uint64_t n;
	while(cin >> n) stones[n]++;

	int STEPS = partb ? 75 : 25;
	for(size_t count = 0; count < STEPS; count++)
	{
		map<uint64_t, size_t> next;
		for(auto& [num, freq] : stones)
		{
			string str = to_string(num);
			if(num == 0)
			{
				next[1] += freq;
			}
			else if(str.size() % 2 == 0)
			{
				uint64_t left = stoull(str.substr(0, str.size() / 2)),
					right = stoull(str.substr(str.size() / 2));
				next[left] += freq;
				next[right] += freq;
			}
			else
			{
				next[num*2024] += freq;
			}
		}
		stones.swap(next);
		next.clear();
	}
	uint64_t c = 0;
	for(auto& [num, freq] : stones)
	{
		c += freq;
	}
	cout << c << "\n";

	return 0;
}

