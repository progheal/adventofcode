#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	bool run = true;
	regex r ("(mul)\\(([0-9]{1,3}),([0-9]{1,3})\\)|(do(n't)?)\\(\\)");
	int64_t total = 0;
	AOC::readPerLine(cin, [&](const string& line){
		smatch m;
		auto p = line.begin();
		while(regex_search(p, line.end(), m, r))
		{
			if(m[1] == "mul")
			{
				if(run)
				{
					int64_t a = stoi(m[2]), b = stoi(m[3]);
					total += a * b;
				}
			}
			else if(partb)
			{
				if(m[4] == "do") run = true;
				else if(m[4] == "don't") run = false;
			}
			p = m.suffix().first;
		}
	});
	cout << total << "\n";

	return 0;
}

