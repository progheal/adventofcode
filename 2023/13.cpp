#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/util.h"
using namespace std;

const size_t NOTFOUND = -1;

size_t differences(const string& a, const string& b)
{
	size_t dif = 0;
	for(size_t i = 0; i < a.size(); i++)
	{
		if(a[i] != b[i]) dif++;
	}
	return dif;
}

size_t findHorizontalMirror(const vector<string>& map, bool smudge)
{
	size_t h = map.size();
	for(size_t i = 1; i < h; i++)
	{
		size_t range = min(i, h-i);
		int sumDif = 0;
		for(size_t j = 0; j < range; j++)
		{
			sumDif += differences(map[i-1-j], map[i+j]);
		}
		if(sumDif == (smudge?1:0)) return i-1;
	}
	return NOTFOUND;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	string everything;
	getline(cin, everything, (char)(-1));

	vector<string> cases = AOC::tokenize(everything, "\n\n");
	int64_t msum = 0;
	for(const string& pic : cases)
	{
		vector<string> image = AOC::readToVector<string>(pic);
		size_t hMirror = findHorizontalMirror(image, partb);
		size_t vMirror = findHorizontalMirror(AOC::rotateCW(image), partb);
		msum += 100*(hMirror+1) + (vMirror+1);
		//cerr << 100*(hMirror+1) + (vMirror+1) << endl;
	}
	cout << msum << endl;

	return 0;
}

