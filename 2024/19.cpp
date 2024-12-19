#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "aoc/aoc.h"
#include "aoc/util.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	auto input = AOC::readPerLineToVector<string>(cin);
	auto towels = AOC::tokenize(input[0], ", ");
	set<string> tw {towels.begin(), towels.end()};
	input.erase(input.begin());

	int count = 0;
	int64_t arrcount = 0;
	for(auto& query : input)
	{
		size_t n = query.size();
		vector<int64_t> dp;
		dp.resize(n+1, 0);
		dp[0] = 1;
		for(int i = 1; i <= n; i++)
		{
			for(int j = 0; j < i; j++)
			{
				if(tw.count(query.substr(j, i-j)) > 0)
				{
					dp[i] += dp[j];
				}
			}
		}
		if(dp[n] > 0)
		{
			++count;
			arrcount += dp[n];
		}
	}
	if(!partb)
		cout << count << "\n";
	else
		cout << arrcount << "\n";

	return 0;
}

