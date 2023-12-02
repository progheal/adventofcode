#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int sumA = 0, sumB = 0;
	AOC::readPerLine(cin, [&](const string& game)
	{
		auto v = AOC::readToVector<string>(game, ':');
		int gn = stoi(v[0].substr(5));
		int gr = 0, gg = 0, gb = 0;
		AOC::read<string>(v[1], [&](const string& pull)
		{
			int r = 0, g = 0, b = 0;
			AOC::read<string>(pull, [&](const string& color)
			{
				auto w = AOC::readToVector<string>(color);
				int count = stoi(w[0]);
				if(w[1] == "red") r = count;
				else if(w[1] == "green") g = count;
				else if(w[1] == "blue") b = count;
			}, ',');
			gr = max(r, gr);
			gg = max(g, gg);
			gb = max(b, gb);
		}, ';');
		if(gr <= 12 && gg <= 13 && gb <= 14) sumA += gn;
		sumB += gr * gg * gb;
	});
	if(!partb)
		cout << sumA << endl;
	else
		cout << sumB << endl;

	return 0;
}

