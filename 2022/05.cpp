#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> input = AOC::readPicture(cin);

	int idx = find_if(input.begin(), input.end(), [](const string& s){return s[1] == '1';}) - input.begin();
	int sz = (input[idx].size() + 1) / 4;
	vector<vector<char>> v (sz);
	for(int i = 0; i < sz; i++)
	{
		for(int p = idx - 1; p >= 0 && input[p][i*4+1] != ' '; p--)
		{
			v[i].push_back(input[p][i*4+1]);
		}
	}

	for(idx += 2; idx < input.size(); ++idx)
	{
		auto [count, src, target] = AOC::takeFirst<3>(AOC::readNumbers(input[idx]));
		--src; --target;
		if(!partb)
		{
			v[target].insert(v[target].end(), v[src].rbegin(), v[src].rbegin() + count);
			v[src].resize(v[src].size() - count);
		}
		else
		{
			v[target].insert(v[target].end(), v[src].end() - count, v[src].end());
			v[src].resize(v[src].size() - count);
		}
	}
	for(auto& s : v)
	{
		cout << s.back();
	}
	cout << endl;

	return 0;
}

