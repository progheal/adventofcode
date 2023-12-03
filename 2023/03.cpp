#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cctype>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> schema = AOC::readPicture(cin);

	map<int, vector<int>> gear;

	int sumA = 0;
	for(size_t r = 0; r < schema.size(); r++)
	{
		for(size_t c = 0; c < schema[r].size(); c++)
		{
			if(!isdigit(schema[r][c])) continue;
			size_t edc = c;
			while(isdigit(schema[r][edc])) edc++;
			int partnum = stoi(schema[r].substr(c, edc-c));
			char symbol = '.';
			// 結束條件不是小於是因為 size_t 無號，-1 會下溢變大
			for(size_t dr = r-1; dr != r+2; dr++)
			{
				if(dr >= schema.size()) continue;
				for(size_t dc = c-1; dc != edc+1; dc++)
				{
					if(dc >= schema[dr].size()) continue;
					if(!isdigit(schema[dr][dc]) && schema[dr][dc] != '.')
						symbol = schema[dr][dc];
					if(schema[dr][dc] == '*')
						gear[dr * 1000 + dc].push_back(partnum);
				}
			}
			if(symbol != '.') sumA += partnum;
			c = edc - 1;
		}
	}
	if(!partb)
		cout << sumA << endl;
	else
	{
		int gearsum = 0;
		for(auto& g : gear)
		{
			if(g.second.size() == 2)
			{
				gearsum += g.second[0] * g.second[1];
			}
		}
		cout << gearsum << endl;
	}

	return 0;
}

