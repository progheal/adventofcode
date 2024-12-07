#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int64_t total = 0;
	//int count = 0;
	AOC::readPerLine(cin, [&](const string& line)
	{
		//++count;
		//cerr << "\r" << ++count << "/???";
		auto v = AOC::readNumbers<int64_t>(line);
		unordered_set<int64_t> before, after;
		before.insert(v[1]);
		string ts = to_string(v[0]);
		for(size_t i = 2; i < v.size(); i++)
		{
			if(!partb)
			{
				for(int64_t p : before)
				{
					if(p+v[i] <= v[0])
						after.insert(p+v[i]);
					if(p*v[i] <= v[0])
						after.insert(p*v[i]);
				}
			}
			else
			{
				for(int64_t p : before)
				{
					if(p+v[i] <= v[0])
						after.insert(p+v[i]);
					if(p*v[i] <= v[0])
						after.insert(p*v[i]);
					int64_t conc = stoll(to_string(p)+to_string(v[i]));
					if(conc <= v[0])
						after.insert(conc);
				}
			}
			before.swap(after);
			after.clear();
		}
		if(before.count(v[0]) > 0) total += v[0];
	});
	cout << total << "\n";

	return 0;
}

