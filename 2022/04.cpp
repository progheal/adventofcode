#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int count = 0, overlap = 0;
	AOC::readPerLineNumbers(cin,
		[&](const vector<int>& token){
			auto [t1l, t1r, t2l, t2r] = AOC::takeFirst<4>(token);
			if(t1l >= t2l && t1r <= t2r || t1l <= t2l && t1r >= t2r)
			   	count++;
			if(t1l <= t2r && t1r >= t2l || t2l <= t1r && t2r >= t1l)
			   	overlap++;
		});
	if(!partb)
		cout << count << endl;
	else
		cout << overlap << endl;

	return 0;
}

