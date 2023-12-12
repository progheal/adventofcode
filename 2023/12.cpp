#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <string_view>
#include "aoc/aoc.h"
using namespace std;

int64_t findperm(const string& s, const vector<int> seg)
{
	map<int, map<int, int64_t>> cache;
	string_view sv = s;
	//cerr << s << " < ";
	//for(int i : seg) cerr << i << " ";
	//cerr << ">: " << endl;
	auto recurse = [&](auto self, int sidx, int segidx) -> int64_t
	{
		int64_t& cacheEntry = cache[sidx][segidx];
		if(cacheEntry != 0) return cacheEntry - 1;
		//int osidx = sidx;
		//cerr << "Compute (" << osidx << ", " << segidx << ")" << endl;
		int64_t result = 0;
		if(sidx == s.size())
		{
			if(segidx == seg.size())
				result = 1;
			else
				result = 0;
			cacheEntry = result + 1;
			//cerr << "!!(" << osidx << ", " << segidx << ") = " << result << endl;
			return result;
		}
		if(segidx == seg.size())
		{
			if(s.find('#', sidx) == string::npos)
				result = 1;
			else
				result = 0;
			cacheEntry = result + 1;
			//cerr << "!(" << osidx << ", " << segidx << ") = " << result << endl;
			return result;
		}
		while(s[sidx] == '.') sidx++;
		int seglen = seg[segidx];
		if(sidx + seglen > s.size())
		{
			result = 0;
			cacheEntry = result + 1;
			//cerr << "#(" << osidx << ", " << segidx << ") = " << result << endl;
			return result;
		}
		int64_t count = 0;
		while(sidx < s.size())
		{
			if(s[sidx] != '.')
			{
				//cerr << "(" << osidx << ", " << segidx << "): Trying " << sidx << endl;
				if(sv.substr(sidx, seglen).find('.') == string::npos && s[sidx + seglen] != '#')
					count += self(self, sidx + seglen + 1, segidx+1);
				if(s[sidx] == '#') break;
			}
			sidx++;
		}
		cacheEntry = count + 1;
		//cerr << "$(" << osidx << ", " << segidx << ") = " << result << endl;
		return count;
	};
	return recurse(recurse, 0, 0);
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	int64_t sum;
	AOC::readPerLine(cin, [&](const string& s)
	{
		size_t sp = s.find(' ');
		string pic = s.substr(0, sp);
		vector<int> seg = AOC::readNumbers(s);
		if(partb)
		{
			string qpic = '?' + pic;
			pic += qpic + qpic + qpic + qpic;
			vector<int> seg5;
			seg5.resize(seg.size() * 5);
			for(int i = 0; i < 5; i++)
			{
				copy(seg.begin(), seg.end(), seg5.begin() + i * seg.size());
			}
			swap(seg, seg5);
		}
		int64_t perms = findperm(pic+".", seg);
		cerr << s << "; " << perms << endl;
		sum += perms;
	});
	cout << sum << endl;

	return 0;
}

