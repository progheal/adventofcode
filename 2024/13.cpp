#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	int argCounter = 0;
	int64_t arg[6];
	int64_t cost = 0;
	for(auto line : AOC::LineReader{cin})
	{
		auto v = AOC::readNumbers<int64_t>(line);
		if(v.size() != 2) continue;
		arg[argCounter] = v[0];
		arg[argCounter+1] = v[1];
		argCounter += 2;
		if(argCounter == 6)
		{
			argCounter = 0;
			if(partb)
			{
				arg[4] += 10'000'000'000'000;
				arg[5] += 10'000'000'000'000;
			}
			//cerr << arg[0] << "a+" << arg[2] << "b=" << arg[4] << ", "
			//	<< arg[1] << "a+" << arg[3] << "b=" << arg[5] << "\n";
			int64_t D = arg[0]*arg[3] - arg[1]*arg[2];
			int64_t Dx = arg[4]*arg[3] - arg[5]*arg[2];
			int64_t Dy = arg[0]*arg[5] - arg[1]*arg[4];
			if(Dx % D != 0 || Dy % D != 0 || Dx < 0 || Dy < 0)
				continue;
			int64_t x = Dx / D, y = Dy / D;
			cost += x * 3 + y;
		}
	}
	cout << cost << "\n";

	return 0;
}

