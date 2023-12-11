#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	vector<string> galaxy = AOC::readPicture(cin);
	vector<int64_t> xC, yC;
	for(int i = 0; i < galaxy.size(); i++)
	{
		for(int j = 0; j < galaxy[i].size(); j++)
		{
			if(galaxy[i][j] == '#')
			{
				xC.push_back(i);
				yC.push_back(j);
			}
		}
	}
	sort(xC.begin(), xC.end());
	sort(yC.begin(), yC.end());
	int64_t expandCoefficient = argc > 1 ? atoi(argv[1]) : 2;
	if(expandCoefficient == 0) expandCoefficient = 2;
	int64_t stride = 0, base = 0;
	int64_t n = xC.size();
	for(int64_t i = 0; i < n; i++)
	{
		if(i > 0)
		{
			stride += max(0L, xC[i] - xC[i-1] - 1) * (i) * (n-i);
			stride += max(0L, yC[i] - yC[i-1] - 1) * (i) * (n-i);
		}
		base += xC[i] * (i*2-n+1);
		base += yC[i] * (i*2-n+1);
	}
	cout << base + stride * (expandCoefficient - 1) << endl;

	return 0;
}

