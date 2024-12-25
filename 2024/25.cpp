#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include "aoc/aoc.h"
using namespace std;

inline bool fit(const array<string, 7>& a, const array<string, 7>& b)
{
	return inner_product(a.begin(), a.end(), b.begin(),
		true, std::logical_and<bool>(),
		[](const string& sa, const string& sb)
		{
			return inner_product(sa.begin(), sa.end(), sb.begin(),
				true, std::logical_and<bool>(),
				[](const char a, const char b)
				{
					return a == '.' || b == '.';
				});
		});
};

int main(int argc, char* argv[])
{
	vector<array<string, 7>> lock, key;
	int counter = 0;
	array<string, 7> form;
	int fits = 0;
	for(auto& line : AOC::LineReader(cin))
	{
		form[counter] = line;
		++counter;
		if(counter == 7)
		{
			if(form[0][0] == '.')
			{
				for(auto& l : lock)
					if(fit(l, form))
						++fits;
				key.push_back(form);
			}
			else
			{
				for(auto& k : key)
					if(fit(form, k))
						++fits;
				lock.push_back(form);
			}
			counter = 0;
		}
	}
	cout << fits << "\n";

	return 0;
}

