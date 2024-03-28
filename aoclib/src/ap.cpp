#include "aoc/ap.h"
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <vector>

static int64_t gcd(int64_t a, int64_t b)
{
    if(b != 0) while((a %= b) && (b %= a));
    return b ? b : a;
}

static int64_t lcm(int64_t a, int64_t b)
{
    return a / gcd(a, b) * b;
}

static std::pair<int64_t, int64_t> extendedgcd(int64_t a, int64_t b)
{
    std::vector<int64_t> q;
    while(a && b)
    {
        q.push_back(a / b);
        a %= b;
        if(a == 0) break;
        q.push_back(b / a);
        b %= a;
        if(b == 0) break;
    }
    int64_t ax, by;
    bool skip = false;
    if(b == 0)
    {
        ax = 1; by = 0;
        goto B;
    }
    else
    {
        ax = 0; by = 1;
    }
    do
    {
        by -= ax * q.back();
        q.pop_back();
        if(q.empty()) break;
    B:  ax -= by * q.back();
        q.pop_back();
    } while(!q.empty());
    return {ax, by};
}

static int64_t mulinv(int64_t a, int64_t m)
{
    auto [x, y] = extendedgcd(a%m, m);
    return x > 0 ? x : x + m;
}

namespace AOC
{

// Calculate apart(p,q) == {u,v} (precondition: q | p)
// such that p = u*v, q|u, and (u,v) = 1.
std::pair<int64_t, int64_t> apart(int64_t p, int64_t q)
{
    int64_t u = q, v = p / q, g;
    while((g = gcd(u,v)) != 1)
    {
        v /= g;
        u *= g;
    }
    return {u, v};
}

AP APIntersection(const AP& ap1, const AP& ap2)
{
    if(ap1.isEmpty()) return ap1;
    if(ap2.isEmpty()) return ap2;
    int64_t i1 = ap1.min(), d1 = ap1.mod(), r1 = ap1.mathrem(),
            i2 = ap2.min(), d2 = ap2.mod(), r2 = ap2.mathrem();
    if(d1 == 1)
    {
        AP ret = ap2;
        if(ret.min() < i1) ret += (i1 - ret.min() + d2 - 1) / d2 * d2;
        return ret;
    }
    if(d2 == 1)
    {
        AP ret = ap1;
        if(ret.min() < i2) ret += (i2 - ret.min() + d1 - 1) / d1 * d1;
        return ret;
    }
    int64_t g = gcd(d1, d2);
    if(g != 1)
    {
        if(r1 % g != r2 % g) return AP::empty();
        int64_t cr = r1 % g;
        auto [d1g, d1l] = apart(d1, g);
        auto [d2g, d2l] = apart(d2, g);
        AP gap = APIntersection(
            AP(d1g/g, (i1-cr)/g),
            AP(d2g/g, (i2-cr)/g)
        );
        return APIntersection(
            AP(d1l, i1),
            AP(d2l, i2),
            gap * g + cr
        );
    }
    int64_t m1 = d2 * mulinv(d2, d1),
            m2 = d1 * mulinv(d1, d2);
    int64_t xr = r1 * m1 + r2 * m2;
    int64_t mi = std::max(i1, i2);
    int64_t xm = d1 * d2;
    if(xr >= mi)
    {
        int64_t q = (xr - mi) / xm;
        xr -= q * xm;
    }
    else
    {
        int64_t q = (mi - xr + xm - 1) / xm;
        xr += q * xm;
    }
    return AP(xm, xr);
}

AP APIntersection(std::initializer_list<AP>& vap)
{
    return std::accumulate(vap.begin(), vap.end(), AP(), 
        static_cast<AP (*)(const AP&, const AP&)>(APIntersection));
}

std::ostream& operator << (std::ostream& out, const AP& ap)
{
    if(ap.diff == 0)
        out << "empty";
    else
    {
        out << ap.diff << "n";
        if(ap.init > 0)
            out << "+" << ap.init;
        else if(ap.init < 0)
            out << ap.init;
    }
    return out;
}

}
