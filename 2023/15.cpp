#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int hash(const std::string& s)
{
	int h = 0;
	for(char c : s)
		h = (h + c) * 17 % 256;
	return h;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	string s;
	getline(cin, s);
	vector<string> inst = AOC::readToVector<string>(s, ',');
	if(!partb)
	{
		int64_t sum = 0;
		for(auto& s : inst) sum += ::hash(s);
		cout << sum << endl;
	}
	else
	{
		array<list<pair<string, int>>, 256> boxes;
		for(auto& s : inst)
		{
			size_t sp = s.find('=');
			if(sp == string::npos) sp = s.find('-');
			string label = s.substr(0, sp);
			int h = ::hash(label);
			auto& box = boxes[h];
			auto p = find_if(box.begin(), box.end(),
				[&](auto& elem){return elem.first == label;});
			if(s[sp] == '-')
			{
				if(p != box.end()) box.erase(p);
			}
			else
			{
				int f = stoi(s.substr(sp+1));
				if(p != box.end())
					p->second = f;
				else
					box.push_back({label, f});
			}
		}
		int sum = 0;
		for(int i = 0; i < 256; i++)
		{
			int pos = 0;
			for(auto& entry : boxes[i])
			{
				sum += (i+1) * (pos+1) * entry.second;
				++pos;
			}
		}
		cout << sum << endl;
	}

	return 0;
}

