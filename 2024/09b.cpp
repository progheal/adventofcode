#include <iostream>
#include <list>
#include <string>
#include "aoc/aoc.h"
using namespace std;

void dumpDisk(list<pair<int64_t, size_t>>& disk)
{
	for(auto [id, sz] : disk)
	{
		cerr << "<" << id << "x" << sz << ">";
	}
	cerr << endl;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);
	string rle;
	cin >> rle;
	list<pair<int64_t, size_t>> disk;
	int64_t id = 0;
	bool content = true;
	for(char c : rle)
	{
		if(content)
		{
			disk.insert(disk.end(), {id, c - '0'});
			++id;
		}
		else
		{
			if(c > '0')
				disk.insert(disk.end(), {-1, c - '0'});
		}
		content = !content;
	}

	// defrag
	auto p = prev(disk.end());
	for(int64_t idnow = id - 1; idnow != 0; --idnow)
	{
		while(p->first != idnow) --p;
		auto q = disk.begin();
		while(q != p && (q->first != -1 || q->second < p->second)) ++q;
		if(q == p) continue;
		q->first = p->first;
		if(q->second > p->second)
		{
			disk.insert(next(q), {-1, q->second - p->second});
			q->second = p->second;
		}
		p->first = -1;
		if(next(p) != disk.end() && next(p)->first == -1)
		{
			p->second += next(p)->second;
			disk.erase(next(p));
		}
		if(p != disk.begin() && prev(p)->first == -1)
		{
			prev(p)->second += p->second;
			p = disk.erase(p);
		}
		//dumpDisk(disk);
	}

	int64_t checksum = 0;
	size_t addr = 0;
	for(auto [id, sz] : disk)
	{
		if(id != -1)
			checksum += id * (addr * sz + sz * (sz - 1) / 2);
		addr += sz;
	}
	cout << checksum << "\n";
	return 0;
}

