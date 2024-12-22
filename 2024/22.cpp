#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
#include "aoc/aoc.h"
using namespace std;

inline uint64_t prng(uint64_t seed)
{
	seed ^= (seed << 6) & 16777215;
	seed ^= seed >> 5;
	seed ^= (seed << 11) & 16777215;
	return seed;
}

/*
string key2offset(uint64_t key)
{
	return 
		to_string((int)(key / 32 / 32 / 32) - 9) + " " +
		to_string((int)(key / 32 / 32 % 32) - 9) + " " +
		to_string((int)(key / 32 % 32) - 9) + " " +
		to_string((int)(key % 32) - 9);
}
*/

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	if(!partb)
	{
		uint64_t total = 0;
		for(auto seed : AOC::Reader<uint64_t>(cin))
		{
			cerr << seed << ": ";
			for(int i = 0; i < 2000; i++) seed = prng(seed);
			cerr << seed << "\n";
			total += seed;
		}
		cout << total << "\n";
	}
	else
	{
		static array<uint64_t, 32*32*32*32> tally = {};
		for(auto seed : AOC::Reader<uint64_t>(cin))
		{
			array<char, 32*32*32*32> mark = {};
			uint64_t prev = seed % 10;
			uint64_t key = 0;
			for(int i = 1; i <= 2000; i++)
			{
				seed = prng(seed);
				uint64_t curr = seed % 10;
				key %= 32*32*32;
				key = key * 32 + (curr + 9 - prev);
				if(i >= 4 && mark[key] == 0)
				{
					//cerr << key2offset(key) << " yields " << p0[i] << "\n";
					mark[key] = 1;
					tally[key] += curr;
				}
				prev = curr;
			}
		}
		uint64_t maxtotal = 0;
		for(uint64_t v : tally) maxtotal = max(maxtotal, v);
		cout << maxtotal << "\n";
	}

	return 0;
}

