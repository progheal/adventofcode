#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <tuple>
#include <optional>
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

optional<tuple<double, double, double, double>> xyIntersect(const Ray& r1, const Ray& r2)
{
	// (x1, y1) + t (vx1, vy1) == (x2, y2) + u (vx2, vy2)
	// x1 + t vx1 == x2 + u vx2
	// y1 + t vy1 == y2 + u vy2
	// t vx1 - u vx2 = x2 - x1
	// t vy1 - u vy2 = y2 - y1
	// By Cramer
	// den = | vx1 -vx2 |
	//       | vy1 -vy2 |
	// numt= | x2-x1 -vx2 |
	//       | y2-y1 -vy2 |
	// numu= | vx1 x2-x1 |
	//       | vy1 y2-y1 |
	int64_t det = r1.vx * r2.vy - r1.vy * r2.vx;
	if(det == 0) return nullopt;
	int64_t dx = r2.x - r1.x, dy = r2.y - r1.y;
	int64_t numt = dx * r2.vy - dy * r2.vx;
	int64_t numu = r1.vx * dy - r1.vy * dx;
	double t = (double)numt / (double) det;
	double u = -(double)numu / (double) det;
	double ix1 = r1.x + r1.vx * t,
	       iy1 = r1.y + r1.vy * t,
		   ix2 = r2.x + r2.vx * u,
		   iy2 = r2.y + r2.vy * u;
	/*
	if(abs(ix1 - ix2)/max(abs(ix1), abs(ix2)) > 1e-5 ||
	   abs(iy1 - iy2)/max(abs(iy1), abs(iy2)) > 1e-5)
	{
		cerr << "Error: " << r1 << " x " << r2 << endl;
		cerr << "t = " << setprecision(15) << t << ": (" << setprecision(15) << ix1 << ", " << setprecision(15) <<iy1 << ")" << endl;
		cerr << "u = " << setprecision(15) << u << ": (" << setprecision(15) << ix2 << ", " << setprecision(15) << iy2 << ")" << endl;
		cerr << "delta = (" << setprecision(15) << (ix1-ix2) << ", " << setprecision(15) << (iy1-iy2) << ")" << endl;
	}
	*/
	return tuple{t, u, ix1, iy1};
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	const double RANGEMIN = 2e14,
				 RANGEMAX = 4e14;

	vector<Ray> hails = AOC::readPerLineToVector<Ray>(cin);

	if(!partb)
	{
		int count = 0;
		for(int i = 0; i < hails.size(); i++)
		{
			for(int j = i+1; j < hails.size(); j++)
			{
				//cerr << hails[i] << " x " << hails[j] << ": " << endl;
				auto arg = xyIntersect(hails[i], hails[j]);
				if(!arg)
				{
					//cerr << "parallel" << endl;
					continue;
				}
				auto [t, u, x, y] = *arg;
				//cerr << "at t = " << setprecision(15) << t
				//	 << ", u = " << setprecision(15) << u
				//	 << ", (" << setprecision(15) << x
				//	 << ", " << setprecision(15) << y << ")";
				if(t < 0 || u < 0)
				{
					//cerr << ", ";
					//if(t < 0) cerr << "past-A";
					//if(t < 0 && u < 0) cerr << ", ";
					//if(u < 0) cerr << "past-B";
					//cerr << endl;
					continue;
				}
				if(x < RANGEMIN || x > RANGEMAX || y < RANGEMIN || y > RANGEMAX)
				{
					//cerr << ", outside" << endl;
					continue;
				}
				//cerr << ", inside" << endl;
				++count;
			}
		}
		cout << count << endl;
	}

	return 0;
}

