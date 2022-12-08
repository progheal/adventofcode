#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
using namespace std;

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> forest = AOC::readPicture(cin);
	int h = forest.size(), w = forest[0].size();

	auto inside = [=](int i, int j)
	{
		return i >= 0 && i < h && j >= 0 && j < w;
	};

	if(!partb)
	{
		vector<vector<int>> v;
		v.resize(h, vector<int>(w, 0));
		char curr;
		auto update = [&](int i, int j)
		{
			if(curr < forest[i][j])
			{
				v[i][j] = 1;
				curr = forest[i][j];
			}
		};
		for(int i = 0; i < h; i++)
		{
			curr = ' ';
			for(int j = 0; j < w; j++)
				update(i, j);
			curr = ' ';
			for(int j = w-1; j >= 0; j--)
				update(i, j);
		}
		for(int j = 0; j < w; j++)
		{
			curr = ' ';
			for(int i = 0; i < h; i++)
				update(i, j);
			curr = ' ';
			for(int i = h-1; i >= 0; i--)
				update(i, j);
		}
		int count = 0;
		for(auto& row : v) for(int x : row) count += x;
		cout << count << endl;
	}
	else
	{
		int maxsc = 0;
		int dir[] = {0, 1, 0, -1, 0};
		for(int i = 1; i < h-1; i++)
		{
			for(int j = 1; j < w-1; j++)
			{
				int sc = 1;
				for(int d = 0; d < 4; d++)
				{
					int di = dir[d], dj = dir[d+1], ni = i, nj = j, x = 0;
					do
					{
						++x;
						ni += di;
						nj += dj;
					} while(inside(ni, nj) && forest[ni][nj] < forest[i][j]);
					if(!inside(ni, nj)) --x;
					sc *= x;
				}
				maxsc = max(maxsc, sc);
			}
		}
		cout << maxsc << endl;
	}

	return 0;
}

