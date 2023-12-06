#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#include "aoc/aoc.h"
using namespace std;

/*
 * t (T-t) = D => t^2 - Tt + D = 0
 * By Vieta, t1 + t2 = T, t1 * t2 = D
 * Therefore |t1 - t2| = sqrt(T^2-4D)
 * # of solutions that t (T-t) > D is between t1 and t2, basically |t1 - t2| + 1
 * For integer solution, |t1 - t2| should be integer & same parity with T
 */
int64_t raceWinCount(int64_t time, int64_t dist)
{
	double qdet = sqrt((double)time * time - 4. * dist);
	int64_t dif = floor(qdet);
	if(dif == qdet) --dif;
	if(time % 2 != dif % 2) --dif;
	return dif + 1;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	if(!partb)
	{
		vector<vector<int>> input = AOC::readNumberMatrix<int>(cin);
		int64_t count = 1;
		for(int i = 0; i < input[0].size(); i++)
		{
			count *= raceWinCount(input[0][i], input[1][i]);
		}
		cout << count << endl;
	}
	else
	{
		vector<int64_t> input;
		AOC::readPerLine(cin, [&](const string& s)
		{
			string t;
			for(char c : s) if(isdigit(c)) t += c;
			input.push_back(stol(t));
		});
		cerr << input[0] << " " << input[1] << endl;

		cout << raceWinCount(input[0], input[1]) << endl;
	}

	return 0;
}

