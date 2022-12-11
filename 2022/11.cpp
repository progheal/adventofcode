#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "aoc/aoc.h"
using namespace std;

struct Monkey
{
	vector<int64_t> holds;
	function<int64_t(int64_t)> inspect;
	int64_t divisor;
	int64_t divisible, nondivisible;
	int64_t inspectCount;
};

int64_t gcd(int64_t a, int64_t b)
{
	if(b) while((a %= b) && (b %= a));
	return b ? b : a;
}

int64_t lcm(int64_t a, int64_t b)
{
	return a / gcd(a, b) * b;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<Monkey> vm;
	int linecount = 0;
	int64_t divlcm = 1;
	AOC::readPerLine(cin,
		[&](const string& line)
		{
			switch(linecount%6)
			{
			case 0:
				vm.push_back(Monkey{});
				break;
			case 1:
				{
					auto nums = AOC::readNumbers(line);
					transform(nums.begin(), nums.end(), back_inserter(vm.back().holds),
						[](int x)->int64_t{return x;});
					break;
				}
			case 2:
				{
					char op = line[21];
					string rhs = line.substr(23);
					if(rhs == "old")
					{
						vm.back().inspect = [](int64_t x){return x*x;};
					}
					else
					{
						int r2 = stoi(rhs);
						if(op == '*')
							vm.back().inspect = [=](int64_t x){return x*r2;};
						else
							vm.back().inspect = [=](int64_t x){return x+r2;};
					}
					break;
				}
			case 3:
				vm.back().divisor = AOC::readNumbers(line)[0];
				divlcm = lcm(divlcm, vm.back().divisor);
				break;
			case 4:
				vm.back().divisible = AOC::readNumbers(line)[0];
				break;
			case 5:
				vm.back().nondivisible = AOC::readNumbers(line)[0];
				break;
			}
			++linecount;
		});

	map<int, int> inspectCount;
	int turn = 20;
	if(partb) turn = 10'000;
	while(turn--)
	{
		for(auto& m : vm)
		{
			m.inspectCount += m.holds.size();
			for(int v : m.holds)
			{
				if(!partb)
					v = m.inspect(v) / 3;
				else
					v = m.inspect(v) % divlcm;
				int target = (v % m.divisor == 0) ? m.divisible : m.nondivisible;
				vm[target].holds.push_back(v);
			}
			m.holds.clear();
		}
	}
	vector<int64_t> v;
	transform(vm.begin(), vm.end(), back_inserter(v),
		[](const Monkey& m){return m.inspectCount;});
	sort(v.begin(), v.end(), std::greater<int>());
	cout << v[0] * v[1] << endl;

	return 0;
}

