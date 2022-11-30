#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/ocr.h"
using namespace std;

int main()
{
	vector<string> glyph = AOC::readToVector<string>(cin, '\n');
	string result = AOC::ocr(glyph);
	cout << result << endl;
	return 0;
}

