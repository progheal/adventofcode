#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

bool isSafe(const vector<int>& v, int ignore = -1)
{
	bool upok = true, downok = true;
	int prev = v[0];
	if(ignore == 0) prev = v[1];
	for(int i = ignore == 0 ? 2 : 1; i < v.size(); i++)
	{
		if(i == ignore) continue;
		int dif = v[i] - prev;
		if(dif > 3 || dif < 1) upok = false;
		if(dif > -1 || dif < -3) downok = false;
		prev = v[i];
	}
	return upok || downok;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	int safe = 0;
	AOC::readPerLine(cin, [&](const string& line){
		auto value = AOC::readNumbers(line);
		if(!partb)
		{
			if(isSafe(value)) ++safe;
		}
		else
		{
			for(int ig = -1; ig < (int)value.size(); ig++)
			{
				if(isSafe(value, ig))
				{
					++safe;
					return;
				}
			}
		}
	});
	cout << safe << "\n";

	return 0;
}

