#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include "aoc/aoc.h"
using namespace std;

int64_t extrapolate(const vector<int64_t>& value, bool left)
{
	vector<int64_t> dif;
	transform(value.begin()+1, value.end(), value.begin(), back_inserter(dif),
		[](int64_t a, int64_t b){return a - b;});
	int64_t difPredict;
	if(all_of(begin(dif), end(dif), [w=dif[0]](int64_t v){return v == w;}))
		difPredict = dif[0];
	else
		difPredict = extrapolate(dif, left);
	return left ? value[0] - difPredict : value.back() + difPredict;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int64_t sum = 0;
	AOC::readPerLine(cin, [&](const string& line)
	{
		vector<int64_t> row = AOC::readToVector<int64_t>(line);
		int64_t predict = extrapolate(row, partb);
		sum += predict;
	});
	cout << sum << endl;

	return 0;
}

