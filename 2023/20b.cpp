#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

struct PeriodicSignal
{
	vector<pair<int64_t, bool>> sequence;
	int64_t period;
};



int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	return 0;
}

