#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include "aoc/aoc.h"
#include "aoc/grid.h"
using namespace std;
using AOC::Coord;

// Calculate winding number to find if point is in polygon
bool inside(const vector<Coord>& polygon, const Coord& p)
{
    int n = polygon.size();
    double anglesum = 0;
    for(int i = 0; i < n; i++)
    {
        int j = (i+1)%n;
        if(polygon[i].x == polygon[j].x && p.x == polygon[i].x &&
            (polygon[i].y <= p.y && p.y <= polygon[j].y ||
             polygon[i].y >= p.y && p.y >= polygon[j].y))
            return true;
        if(polygon[i].y == polygon[j].y && p.y == polygon[i].y &&
            (polygon[i].x <= p.x && p.x <= polygon[j].x ||
             polygon[i].x >= p.x && p.x >= polygon[j].x))
            return true;
        auto v1 = polygon[i] - p, v2 = polygon[j] - p;
        int64_t inner = (int64_t)v1.x * v2.x + (int64_t)v1.y * v2.y;
        int64_t outer = (int64_t)v1.x * v2.y - (int64_t)v2.x * v1.y;
        double c = inner / v1.euclideanDistance() / v2.euclideanDistance();
        anglesum += (outer >= 0 ? 1 : -1) * acos(c);
    }
    // In theory, anglesum should be multiple of 2pi (=4*acos(0))
    // so divide by 2pi than round to account for tiny inaccuracy of acos
    return round(anglesum / (4.0*acos(0.0))) != 0;
}


int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    vector<AOC::Coord> reds;
    vector<int> xs, ys;
    string line;
    while(getline(cin, line))
    {
        AOC::Coord c;
        tie(c.x, c.y) = AOC::takeFirst<2>(AOC::readNumbers(line, true));
        reds.push_back(c);
        xs.push_back(c.x);
        ys.push_back(c.y);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int64_t maxsize = 0;
    for(int i = 0; i < reds.size(); i++)
    {
        for(int j = i+1; j < reds.size(); j++)
        {
            auto v = reds[i] - reds[j];
            // fast reject area too small
            int64_t area = (int64_t)(abs(v.x) + 1) * (int64_t)(abs(v.y) + 1);
            if(maxsize >= area)
            {
                // cerr << reds[i] << "-" << reds[j] << " excluded because it has smaller area " << area << "\n";
                continue;
            }
            // heuristic:
            // any red strictly inside region counts as not full
            if(any_of(reds.begin(), reds.end(), [&](const auto& c){
                auto ret =
                    (reds[i].x < c.x && c.x < reds[j].x ||
                     reds[i].x > c.x && c.x > reds[j].x) &&
                    (reds[i].y < c.y && c.y < reds[j].y ||
                     reds[i].y > c.y && c.y > reds[j].y);
                // if(ret)
                // {
                //     cerr << reds[i] << "-" << reds[j] << " excluded because " << c << " is inside.\n";
                // }
                return ret;
            }))
                continue;
            // heuristic: all edge should be inside polygon
            // only cherry-picking "critical" coordinates
            int xm = min(reds[i].x,reds[j].x),
                ym = min(reds[i].y, reds[j].y),
                xM = max(reds[i].x,reds[j].x),
                yM = max(reds[i].y, reds[j].y);
            auto xmp = lower_bound(xs.begin(), xs.end(), xm),
                 xMp = upper_bound(xs.begin(), xs.end(), xM),
                 ymp = lower_bound(ys.begin(), ys.end(), ym),
                 yMp = upper_bound(ys.begin(), ys.end(), yM);
            bool edgecheck = true;
            for(auto xp = xmp; xp != xMp; xp++)
            {
                if(!inside(reds, {*xp, ym}) || !inside(reds, {*xp, yM}))
                {
                    // cerr << reds[i] << "-" << reds[j] << " excluded because " << Coord{*xp, ym} << " or " << Coord{*xp, yM} << " is outside\n";
                    edgecheck = false;
                    break;
                }
            }
            if(!edgecheck) continue;
            for(auto yp = ymp; yp != yMp; yp++)
            {
                if(!inside(reds, {xm, *yp}) || !inside(reds, {xM, *yp}))
                {
                    // cerr << reds[i] << "-" << reds[j] << " excluded because " << Coord{xm, *yp} << " or " << Coord{xM, *yp} << " is outside\n";
                    edgecheck = false;
                    break;
                }
            }
            if(!edgecheck) continue;
            maxsize = area;
            // cerr << reds[i] << "-" << reds[j] << " has larger area " << area << "\n";
        }
    }
    cout << maxsize << "\n";
}