#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int partASum = 0;
int partBTarget = 0;
int partBSize = 0;

struct Directory
{
	map<string, int> files;
	map<string, Directory> child;
	Directory* parent;
	int size()
	{
		int s = 0;
		for(auto& [name, dir] : child)
		{
			s += dir.size();
		}
		for(auto& [name, sz] : files)
		{
			s += sz;
		}
		if(s <= 100'000) partASum += s;
		if(s > partBTarget) partBSize = min(partBSize, s);
		return s;
	}
};

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	Directory root;
	Directory* cwd = &root;
	root.parent = nullptr;

	AOC::readPerLineTokenized(cin,
		[&](const vector<string>& token)
		{
			if(token[0] == "$")
			{
				if(token[1] == "cd")
				{
					if(token[2] == "/")
					{
						cwd = &root;
					}
					else if(token[2] == "..")
					{
						cwd = cwd->parent;
					}
					else
					{
						cwd = &cwd->child[token[2]];
					}
				}
				// ignore ls command
			}
			else if(token[0] == "dir")
			{
				cwd->child[token[1]].parent = cwd;
			}
			else
			{
				cwd->files[token[1]] = stoi(token[0]);
			}
		});

	int cap = 70'000'000;
	int target = 30'000'000;
	int fsz = root.size();
	if(!partb)
	{
		cout << partASum << endl;
	}
	else
	{
		partBTarget = target - (cap - fsz);
		partBSize = cap;
		root.size();
		cout << partBSize << endl;
	}

	return 0;
}

