#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	vector<int> L, R;
	int a, b;
	while(cin >> a >> b)
	{
		L.push_back(a);
		R.push_back(b);
	}
	if(!partb)
	{
		sort(L.begin(), L.end());
		sort(R.begin(), R.end());
		int ans = transform_reduce(L.begin(), L.end(), R.begin(), 0,
			[](int a, int b){return a+b;},
			[](int a, int b){return abs(a-b);});
		cout << ans << "\n";
	}
	else
	{
		map<int, int> tally;
		for(int x : R) tally[x]++;
		int ans = 0;
		for(int x : L) ans += x * tally[x];
		cout << ans << endl;
	}

	return 0;
}

