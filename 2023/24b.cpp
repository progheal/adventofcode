#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <array>
#include <tuple>
#include "aoc/aoc.h"
using namespace std;

struct Ray
{
	int64_t x, y, z, vx, vy, vz;
};

namespace AOC
{
template<>
Ray convert<Ray>(const string& s)
{
	Ray r;
	tie(r.x, r.y, r.z, r.vx, r.vy, r.vz) = AOC::takeFirst<6>(AOC::readNumbers<int64_t>(s, true));
	return r;
}
}

ostream& operator << (ostream& out, const Ray& r)
{
	out << "(" << r.x << ", " << r.y << ", " << r.z << " @ "
		<< r.vx << ", " << r.vy << ", " << r.vz << ")";
	return out;
}

// 高斯消去的中間結果 64-bit 也爆了，所以用 128-bit
array<__int128_t, 5> TwoRayToEquation(const Ray& r1, const Ray& r2)
{
	return {
		r1.vy - r2.vy,
		r2.vx - r1.vx,
		r2.y - r1.y,
		r1.x - r2.x,
		r1.x*r1.vy - r2.x*r2.vy - r1.y*r1.vx + r2.y*r2.vx
	};
}

// 但是標準函式庫沒有給 128-bit 的函數，所以只好自己寫一個，包含絕對值...
__int128_t abs(__int128_t value)
{
	return value < 0 ? -value : value;
}

// ...以及輸出
ostream& operator << (ostream& out, __int128_t value)
{
	if(static_cast<__int128_t>(static_cast<int64_t>(value)) == value)
		return out << static_cast<int64_t>(value);
	// 去掉後九位再試試
	__int128_t top = value / 1'000'000'000;
	out << top;
	int64_t remain = static_cast<int64_t>(value - top * 1'000'000'000);
	out << setw(9) << setfill('0') << abs(remain);
	return out;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<Ray> hails = AOC::readPerLineToVector<Ray>(cin);

	// 實作這個連結裡的演算法
	// https://www.reddit.com/r/adventofcode/comments/18pnycy/2023_day_24_solutions/kesvc7t/
	array<array<__int128_t, 5>, 4> eq = {
		TwoRayToEquation(hails[0], hails[1]),
		TwoRayToEquation(hails[2], hails[3]),
		TwoRayToEquation(hails[4], hails[5]),
		TwoRayToEquation(hails[6], hails[7])
	};

#ifdef DEBUG
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			cout << eq[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
#endif

	// 輾轉高斯消去法，很沒效率但只要用整數
	for(int i = 0; i < 4; i++)
	{
		while(true)
		{
			size_t zero = 0;
			for(int j = i+1; j < 4; j++)
			{
				if(eq[j][i] != 0 && abs(eq[j][i]) < abs(eq[i][i]))
					swap(eq[i], eq[j]);
				else if(eq[j][i] == 0)
					++zero;
			}
			if(zero == 3-i) break;
			for(int j = i+1; j < 4; j++)
			{
				int q = eq[j][i] / eq[i][i];
				for(int k = 0; k < 5; k++)
				{
					eq[j][k] -= q * eq[i][k];
				}
			}
		#ifdef DEBUG
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 5; j++)
				{
					cout << eq[i][j] << " ";
				}
				cout << endl;
			}
			cout << endl;
		#endif
		}
	}

	// 倒回代換
	for(int i = 3; i >= 0; i--)
	{
		if(eq[i][4] % eq[i][i] != 0)
		{
			cout << "Error: not evenly divisible" << endl;
			return 0;
		}
		eq[i][4] /= eq[i][i];
		eq[i][i] = 1;
		for(int k = 0; k < i; k++)
		{
			for(int j = 4; j >= i; j--)
			{
				eq[k][j] -= eq[k][i] * eq[i][j];
			}
		}
	#ifdef DEBUG
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				cout << eq[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	#endif
	}

	Ray stone;
	stone.x = eq[0][4];
	stone.y = eq[1][4];
	stone.vx = eq[2][4];
	stone.vy = eq[3][4];

#ifdef DEBUG
	cout << "x = " << stone.x << ", y = " << stone.y
		<< ", vx = " << stone.vx << ", vy = " << stone.vy << endl;
	cout << "Finding stone z" << endl;
#endif

	int64_t num = stone.x - hails[0].x;
	int64_t den = hails[0].vx - stone.vx;
	if(num % den != 0)
	{
		cout << "Error: t0 not divisible" << endl;
		return 0;
	}
	int64_t t0 = num / den;
#ifdef DEBUG
	cout << "t0 = " << t0 << endl;
#endif
	num = stone.x - hails[1].x;
	den = hails[1].vx - stone.vx;
	if(num % den != 0)
	{
		cout << "Error: t1 not divisible" << endl;
		return 0;
	}
	int64_t t1 = num / den;
#ifdef DEBUG
	cout << "t1 = " << t1 << endl;
#endif
	num = hails[1].z - hails[0].z - t0 * hails[0].vz + t1*hails[1].vz;
	den = t1 - t0;
	if(num % den != 0)
	{
		cout << "Error: vz not divisible" << endl;
		return 0;
	}
	stone.vz = num / den;
#ifdef DEBUG
	cout << "vz = " << stone.vz << endl;
#endif
	stone.z = t0 * (hails[0].vz - stone.vz) + hails[0].z;
#ifdef DEBUG
	cout << "z = " << stone.z << endl << endl;
#endif

#ifdef DEBUG
	cout << "Final answer = ";
#endif
	cout << (stone.x + stone.y + stone.z) << endl;

	return 0;
}

