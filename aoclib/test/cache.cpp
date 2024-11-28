#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "aoc/cache.h"
using namespace std;
using namespace testing;

vector<int> oneHistory;

int64_t one(int n)
{
    oneHistory.push_back(n);
    return n+1;
}

vector<pair<int, int>> twoHistory;

int64_t two(int a, int b)
{
    twoHistory.push_back({a, b});
    return a+b;
}

TEST(CacheTest, One)
{
    auto oneCached = AOC::cache(one);
    oneHistory.clear();
    vector<int> result;
    for(int arg : {1,2,3,1,3,1,3,2,3,4,4,3,2,4})
    {
        result.push_back(oneCached(arg));
    }
    EXPECT_THAT(result,
        ::testing::ContainerEq(vector<int>{2,3,4,2,4,2,4,3,4,5,5,4,3,5}));
    EXPECT_THAT(oneHistory,
        ::testing::ContainerEq(vector<int>{1,2,3,4}));
}

TEST(CacheTest, Two)
{
    auto twoCached = AOC::cache(two);
    twoHistory.clear();
    vector<int> result;
    vector<pair<int, int>> args = {{1,2},{3,1},{3,1},{3,2},{3,4},{3,4},{2,4}};
    for(auto& [arg1, arg2] : args)
    {
        result.push_back(twoCached(arg1, arg2));
    }
    EXPECT_THAT(result,
        ::testing::ContainerEq(vector<int>{3,4,4,5,7,7,6}));
    EXPECT_THAT(twoHistory,
        ::testing::ContainerEq(vector<pair<int, int>>{{1,2},{3,1},{3,2},{3,4},{2,4}}));
}

TEST(CacheTest, OneLambda)
{
    auto oneCached = AOC::cache([](int n) -> int64_t
    {
        oneHistory.push_back(n);
        return n+1;
    });
    oneHistory.clear();
    vector<int> result;
    for(int arg : {1,2,3,1,3,1,3,2,3,4,4,3,2,4})
    {
        result.push_back(oneCached(arg));
    }
    EXPECT_THAT(result,
        ::testing::ContainerEq(vector<int>{2,3,4,2,4,2,4,3,4,5,5,4,3,5}));
    EXPECT_THAT(oneHistory,
        ::testing::ContainerEq(vector<int>{1,2,3,4}));
}

TEST(CacheTest, TwoLambda)
{
    auto twoCached = AOC::cache([](int a, int b) -> int64_t
    {
        twoHistory.push_back({a, b});
        return a+b;
    });
    twoHistory.clear();
    vector<int> result;
    vector<pair<int, int>> args = {{1,2},{3,1},{3,1},{3,2},{3,4},{3,4},{2,4}};
    for(auto& [arg1, arg2] : args)
    {
        result.push_back(twoCached(arg1, arg2));
    }
    EXPECT_THAT(result,
        ::testing::ContainerEq(vector<int>{3,4,4,5,7,7,6}));
    EXPECT_THAT(twoHistory,
        ::testing::ContainerEq(vector<pair<int, int>>{{1,2},{3,1},{3,2},{3,4},{2,4}}));
}

TEST(CacheTest, OneRecursiveLambda)
{
    auto oneCached = AOC::cache_recursive([](auto self, int n) -> int64_t
    {
        oneHistory.push_back(n);
        if(n <= 2) return 1;
        return self(self, n-1) + self(self, n-2);
    });
    oneHistory.clear();
    vector<int> result;
    for(int arg : {10,9,8,7,6,5,4,3,2,1})
    {
        result.push_back(oneCached(arg));
    }
    EXPECT_THAT(result,
        ::testing::ContainerEq(vector<int>{55,34,21,13,8,5,3,2,1,1}));
    EXPECT_THAT(oneHistory,
        ::testing::ContainerEq(vector<int>{10,9,8,7,6,5,4,3,2,1}));
}

TEST(CacheTest, TwoRecursiveLambda)
{
    auto twoCached = AOC::cache_recursive([](auto self, int m, int n) -> int64_t
    {
        twoHistory.push_back({m, n});
        if(m == 0 || n == 0) return 1;
        return self(self, m, n-1) + self(self, m-1, n);
    });
    twoHistory.clear();
    vector<int> result;
    vector<pair<int, int>> args = {{0, 0}, {1, 1}, {3, 2}, {2, 4}, {2, 2}};
    for(auto& [arg1, arg2] : args)
    {
        result.push_back(twoCached(arg1, arg2));
    }
    EXPECT_THAT(result,
        ::testing::ContainerEq(vector<int>{1, 2, 10, 15, 6}));
    EXPECT_THAT(twoHistory,
        ::testing::ContainerEq(vector<pair<int,int>>{
            {0,0},
            {1,1},{1,0},{0,1},
            {3,2},{3,1},{3,0},{2,1},{2,0},{2,2},{1,2},{0,2},
            {2,4},{2,3},{1,3},{0,3},{1,4},{0,4}
        }));
}
