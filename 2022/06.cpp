#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int difc = 4;
	if(partb) difc = 14;

	string s;
	cin >> s;
	for(size_t i = difc-1; ; i++)
	{
		bool check = true;
		for(size_t k = 0; k < difc; k++)
		{
			for(size_t l = k+1; l < difc; l++)
			{
				if(s[i-k] == s[i-l])
				{
					check = false;
				}
			}
		}
		if(check)
		{
			cout << i+1 << endl;
			return 0;
		}
	}

	return 0;
}

