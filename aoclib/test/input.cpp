#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include "aoc/aoc.h"
using namespace std;
using namespace testing;

TEST(InputTest, Convert)
{
    EXPECT_EQ(AOC::convert<int>("42"s), 42);
    EXPECT_EQ(AOC::convert<int>("-9"s), -9);
    EXPECT_EQ(AOC::convert<unsigned int>("2147483648"s), 2147483648u);
    EXPECT_EQ(AOC::convert<int64_t>("123456789012345"s), INT64_C(123456789012345));
    EXPECT_EQ(AOC::convert<uint64_t>("12345678901234567890"s), UINT64_C(12345678901234567890));
    EXPECT_EQ(AOC::convert<float>("123.625"s), 123.625f);
    EXPECT_EQ(AOC::convert<double>("12345.15625"s), 12345.15625);
    EXPECT_EQ(AOC::convert<string>("foo"s), "foo"s);
}

template<class T>
class InputMatches
{
    vector<T> expected;
    size_t curr = 0;
    bool copiedBeforeCalled = false;
public:
    InputMatches(const vector<T>& v) : expected(v) {}
    InputMatches(std::initializer_list<T> v) : expected(v) {}
    InputMatches(InputMatches& rhs) : expected(rhs.expected), curr(rhs.curr)
    {
        if(rhs.curr == 0) rhs.copiedBeforeCalled = true;
    }
    InputMatches(InputMatches&& rhs) : expected(rhs.expected), curr(rhs.curr)
    {
        if(rhs.curr == 0) rhs.copiedBeforeCalled = true;
    }
    void operator ()(const T& token)
    {
        ASSERT_LT(curr, expected.size());
        EXPECT_EQ(token, expected[curr]);
        ++curr;
    }
    ~InputMatches()
    {
        if(!copiedBeforeCalled)
            EXPECT_EQ(curr, expected.size());
    }
};

template<class T>
InputMatches(const vector<T>&) -> InputMatches<T>;

TEST(InputTest, Read)
{
    string test = "Advent of Code\n\nMerry Christmas"s;
    AOC::read<string>(test,
        InputMatches{"Advent"s, "of"s, "Code"s, "Merry"s, "Christmas"s});

    EXPECT_EQ(AOC::readToVector<string>(test),
        (vector{"Advent"s, "of"s, "Code"s, "Merry"s, "Christmas"s}));

    AOC::read<string>(test, 
        InputMatches{"Advent of Code"s, "Merry Christmas"s}, '\n');

    istringstream iss1(test);
    AOC::readPerLine(iss1,
        InputMatches{"Advent of Code"s, "Merry Christmas"s});

    AOC::read<string>(test, 
        InputMatches{"Advent of Code"s, ""s, "Merry Christmas"s}, '\n', true);

    istringstream iss2(test);
    AOC::readPerLineTokenized(iss2,
        InputMatches{
            vector{"Advent"s, "of"s, "Code"s},
            vector{"Merry"s, "Christmas"s}
        });
}


TEST(InputTest, ReadPicture)
{
    string maze =
R"(#######
#......
#.#####
#...#  
###.#  
  #.#  )";
    istringstream is (maze);
    vector<string> tokenMaze = {
        "#######",
        "#......",
        "#.#####",
        "#...#  ",
        "###.#  ",
        "  #.#  "
    };
    EXPECT_EQ(AOC::readPicture(is), tokenMaze);
}

TEST(InputTest, Numbers)
{
    string numbers = R"(On the twelfth day of Christmas my true love sent to me
12 drummers drumming
11 pipers piping
10 lords a-leaping
9 ladies dancing
8 maids a-milking
7 swans a-swimming
6 geese a-laying
5 gold rings
4 calling birds
3 French hens,
2 turtle doves,
And a partridge in a pear tree.)";
    EXPECT_EQ(AOC::readNumbers(numbers),
        (vector<int>{12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2}));
    
    vector<vector<int>> vNumberMatrix {
            vector<int>{}, vector<int>{12}, vector<int>{11}, 
            vector<int>{10}, vector<int>{9}, vector<int>{8}, 
            vector<int>{7}, vector<int>{6}, vector<int>{5}, 
            vector<int>{4}, vector<int>{3}, vector<int>{2}, vector<int>{}
        };
    istringstream iss(numbers);
    AOC::readPerLineNumbers(iss, InputMatches(vNumberMatrix));

    EXPECT_EQ(AOC::readNumberMatrix(numbers), vNumberMatrix);
    
    string numberMatrix = R"(1 2 3 4
2 3 4 5
3 4 5 6
4 5 6 7)";
    EXPECT_EQ(AOC::readToMatrix<int>(numberMatrix), 
        (vector{
            vector{1,2,3,4}, vector{2,3,4,5}, vector{3,4,5,6}, vector{4,5,6,7}
        }));

    string largeNum = "123456789012 234567890123";
    EXPECT_EQ(AOC::readNumbers<int64_t>(largeNum),
        (vector<int64_t>{123456789012LL, 234567890123LL}));
}

TEST(InputTest, TakeFirst)
{
    vector<int> v = {1, 2, 3, 4, 5};
    auto [x, y] = AOC::takeFirst<2>(v);
    EXPECT_EQ(x, 1);
    EXPECT_EQ(y, 2);

    int p, q, r;
    std::tie(p, q, r) = AOC::takeFirst<3>(v);
    EXPECT_EQ(p, 1);
    EXPECT_EQ(q, 2);
    EXPECT_EQ(r, 3);
}
