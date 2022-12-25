#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	int64_t sum = 0;

	AOC::readPerLine(cin,
		[&](const string& s)
		{
			int64_t n = 0;
			for(char c : s)
			{
				switch(c)
				{
				case '0': case '1': case '2':
					n = n * 5 + (c - '0');
					break;
				case '-':
					n = n * 5 - 1;
					break;
				case '=':
					n = n * 5 - 2;
					break;
				}
			}
			sum += n;
		});

	string t;
	while(sum != 0)
	{
		int64_t r = sum % 5;
		if(r < 0) r += 5;
		t = ("012=-"[r]) + t;
		if(r > 2) r -= 5;
		sum = (sum - r) / 5;
	}
	cout << t << endl;

	return 0;
}

