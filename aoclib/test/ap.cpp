#include <gtest/gtest.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include "aoc/ap.h"
using namespace std;
using namespace testing;

typedef std::tuple<AOC::AP, AOC::AP> testdata_t;

class APCalculation : public ::testing::TestWithParam<testdata_t>
{
};

TEST_P(APCalculation, calculate)
{
    auto& [u, v] = GetParam();
    auto uv = AOC::APIntersection(u, v);
    if(uv.isEmpty())
    {
        int64_t mm = std::max(u.min(), v.min());
        int64_t range = u.mod() * v.mod();
        for(int64_t i = 0; i < range; i++)
        {
            EXPECT_FALSE(u.has(mm+i) && v.has(mm+i))
                << (mm+i) << " belongs to both " << u << " and " << v;
        }
    }
    else
    {
        auto prev = uv.min() - uv.mod();
        EXPECT_FALSE(u.has(prev) && v.has(prev))
            << "prev term of " << uv << " (" << prev
            << ") is in both " << u << " and " << v;
        int64_t mm = std::max(u.min(), v.min());
        int64_t range = u.mod() * v.mod();
        for(int64_t i = 0; i < range; i++)
        {
            bool origin = u.has(mm+i) && v.has(mm+i);
            bool calc = uv.has(mm+i);
            EXPECT_EQ(origin, calc)
                << (mm+i) << " is " << (origin ? "" : "not ") << "common between "
                << u << " and " << v << ", but is " << (calc ? "" : "not ")
                << "in " << uv;
        }
    }
}

std::string testName(const ::testing::TestParamInfo<testdata_t>& info)
{
    std::stringstream ss;
    auto& [u, v] = info.param;
    ss << u << "_" << v;
    std::string s = ss.str();
    s.erase(std::remove(s.begin(), s.end(), '+'), s.end());
    for(char& c : s) if(c == '-') c = '_';
    return s;
}

auto getAP(int64_t max)
{
    vector<AOC::AP> vec;
    for(int64_t d = 2; d <= max; d++) for(int64_t r = 0; r <= d*2; r++)
    {
        vec.emplace_back(d, r);
    }
    return vec;
}

INSTANTIATE_TEST_SUITE_P(APTest, APCalculation,
    ::testing::Combine(
        ::testing::ValuesIn(getAP(20)),
        ::testing::ValuesIn(getAP(20))
    ), testName);
