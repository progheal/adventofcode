#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	map<int, int> copies;

	int pointsum = 0;
	int cards = 0;
	AOC::readPerLine(cin, [&](const string& card)
	{
		size_t colon = card.find(':');
		int cnum = stoi(card.substr(5, colon-5));
		copies[cnum]++;
		cards += copies[cnum];
		size_t bar = card.find('|');
		auto winning = AOC::readToVector<int>(card.substr(colon+1, bar-colon-1));
		auto scratch = AOC::readToVector<int>(card.substr(bar+1));
		set<int> scrset (scratch.begin(), scratch.end());
		int count = 0;
		for(int w : winning) if(scrset.count(w) != 0) ++count;
		if(count > 0)
		{
			pointsum += (1 << (count - 1));
			for(int i = 1; i <= count; i++)
			{
				copies[cnum+i] += copies[cnum];
			}
		}
	});
	if(!partb)
		cout << pointsum << endl;
	else
		cout << cards << endl;

	return 0;
}

