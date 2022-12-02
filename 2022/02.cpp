#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int score = 0;
	AOC::readPerLineTokenized(cin, [&](const vector<string>& s)
	{
		int opp = s[0][0] - 'A';
		int inst = s[1][0] - 'X';
		if(!partb)
			score += (inst + 1) + (inst - opp + 4) % 3 * 3;
		else
			score += (opp + inst + 2) % 3 + 1 + inst * 3;
	});
	cout << score << endl;

	return 0;
}

