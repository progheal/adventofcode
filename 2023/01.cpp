#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	string words[] = {"", "one", "two", "three", "four",
		"five", "six", "seven", "eight", "nine"};

	int sum = 0;
	AOC::readPerLine(cin, [&](const string& line)
	{
		size_t p = line.find_first_of("0123456789"),
		       q = line.find_last_of("0123456789");
		int vp = 0, vq = 0;
		if(p != string::npos)
		{
			vp = line[p] - '0';
			vq = line[q] - '0';
		}
		else
		{
			// Part 1 保證這裡不會進來，不過還是補一下
			p = line.size();
			q = 0;
		}
		if(partb)
		{
			// string::substr() 太貴了，用 string_view::substr() 比較便宜
			string_view sv = line;
			for(size_t i = 0; i < p; i++)
			{
				auto ps = find_if(begin(words)+1, end(words),
					[&](const string& dn){
						return sv.substr(i, dn.size()) == dn;
					}) - words;
				if(ps < 10)
				{
					vp = ps;
					break;
				}
			}
			// j < line.size() 在 j 下溢到 -1 時為 false
			for(size_t j = line.size() - 1; j >= q && j < line.size(); j--)
			{
				auto ps = find_if(begin(words)+1, end(words),
					[&](const string& dn){
						return sv.substr(j, dn.size()) == dn;
					}) - words;
				if(ps < 10)
				{
					vq = ps;
					break;
				}
			}
		}
		int value = vp * 10 + vq;
		sum += value;
	});

	cout << sum << endl;

	return 0;
}

