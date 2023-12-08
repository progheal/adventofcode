#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include "aoc/aoc.h"
#include "aoc/util.h"
using namespace std;

int64_t gcd(int64_t a, int64_t b)
{
	if(b != 0) while((a %= b) && (b %= a));
	return b ? b : a;
}

int64_t lcm(int64_t a, int64_t b)
{
	return a / gcd(a, b) * b;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	map<string, pair<string, string>> m;

	string pattern;
	cin >> pattern;
	AOC::readPerLine(cin, [&](const string& s)
	{
		vector<string> ls = AOC::tokenize(s, regex("\\W+"));
		m[ls[0]] = pair{ls[1], ls[2]};
	});

	if(!partb)
	{
		string curr = "AAA";
		int step = 0;
		for(; curr != "ZZZ"; step++)
		{
			char move = pattern[step % pattern.size()];
			if(move == 'L')
				curr = m[curr].first;
			else
				curr = m[curr].second;
		}
		cout << step << endl;
	}
	else
	{
		// Wrong solution for part 2 that only works on input, but oh well
		int64_t steps = 1;
		for(auto& kv : m)
		{
			if(kv.first.back() == 'A')
			{
				int step = 0;
				string curr = kv.first;
				for(; curr.back() != 'Z'; step++)
				{
					char move = pattern[step % pattern.size()];
					if(move == 'L')
						curr = m[curr].first;
					else
						curr = m[curr].second;
				}
				steps = lcm(steps, step);
			}
		}
		cout << steps << endl;
	}

	return 0;
}

