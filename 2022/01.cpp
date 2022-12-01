#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<int> calories;
	int curr = 0;
	AOC::readPerLine(cin, [&](const string& s)
	{
		if(s == "")
		{
			calories.push_back(curr);
			curr = 0;
		}
		else
		{
			curr += stoi(s);
		}
	}, true);
	calories.push_back(curr);
	sort(calories.begin(), calories.end(), std::greater<int>());
	if(!partb)
	{
		cout << calories[0] << endl;
	}
	else
	{
		cout << calories[0] + calories[1] + calories[2] << endl;
	}

	return 0;
}

