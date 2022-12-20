#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <list>
#include "aoc/aoc.h"
using namespace std;

void mix(vector<int64_t>& number, int rounds)
{
	int n = number.size();
	list<int> ptr(n);
	vector<list<int>::iterator> hash;
	iota(ptr.begin(), ptr.end(), 0);
	for(auto p = ptr.begin(); p != ptr.end(); p++) hash.push_back(p);

	while(rounds--)
	{
		for(int pv = 0; pv < n; ++pv)
		{
			auto p = hash[pv];
			int64_t num = number[pv];
			num %= n-1;
			if(num < 0) num += n-1;
			if(num != 0)
			{
				p = ptr.erase(p);
				if(num > 0)
				{
					while(num--)
					{
						if(p == ptr.end()) p = ptr.begin();
						++p;
					}
				}
				hash[pv] = ptr.insert(p, pv);
			}
		}
	}
	vector<int64_t> ret;
	for(auto p = ptr.begin(); p != ptr.end(); p++)
	{
		ret.push_back(number[*p]);
	}
	number.swap(ret);
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<int64_t> num = AOC::readToVector<int64_t>(cin);
	int rounds = 1;

	if(partb)
	{
		for(int64_t& i : num) i *= 811589153;
		rounds = 10;
	}

	size_t n = num.size();
	mix(num, rounds);

	size_t z = find(num.begin(), num.end(), 0) - num.begin();
	int64_t z1k = num[(z + 1000) % n];
	int64_t z2k = num[(z + 2000) % n];
	int64_t z3k = num[(z + 3000) % n];
	cout << z1k + z2k + z3k << endl;

	return 0;
}

