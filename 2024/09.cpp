#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	string rle;
	cin >> rle;
	vector<int64_t> disk;
	int64_t id = 0;
	bool content = true;
	for(char c : rle)
	{
		if(content)
		{
			for(int i = 0; i < c - '0'; i++)
				disk.push_back(id);
			++id;
		}
		else
		{
			for(int i = 0; i < c - '0'; i++)
				disk.push_back(-1);
		}
		content = !content;
	}

	// defrag
	size_t N = disk.size();
	size_t p = 0, q = N - 1;
	while(true)
	{
		while(p < N && disk[p] != -1) ++p;
		while(disk[q] == -1) --q;
		if(p >= q) break;
		disk[p] = disk[q];
		disk[q] = -1;
	}

	int64_t checksum = 0;
	for(size_t i = 0; disk[i] != -1; i++)
	{
		checksum += disk[i] * i;
	}
	cout << checksum << "\n";
	return 0;
}

