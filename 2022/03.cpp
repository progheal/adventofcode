#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int priority(char c)
{
	if(c >= 'a' && c <= 'z') return c - 'a' + 1;
	else return c - 'A' + 27;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int sum = 0;
	int count = 0;
	string curr;
	AOC::readPerLine(cin, [&](const string& s)
		{
			if(!partb)
			{
				string l = s.substr(0, s.size()/2);
				string r = s.substr(s.size()/2);
				size_t z = l.find_first_of(r);
				sum += priority(l[z]);
			}
			else
			{
				switch(count % 3)
				{
				case 0: curr = s; break;
				case 2:
				{
					size_t z = curr.find_first_of(s);
					sum += priority(curr[z]);
					break;
				}
				case 1:
				{
					string t = "";
					for(char c : s)
					{
						if(curr.find(c) != string::npos) t += c;
					}
					curr = t;
					break;
				}
				}
			}
			++count;
		});
	cout << sum << endl;

	return 0;
}

