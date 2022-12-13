#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "aoc/util.h"
using namespace std;

TEST(UtilTest, Tokenize)
{
	string s1 = "Hello, World!";
	EXPECT_THAT(AOC::tokenize(s1, "o"),
		::testing::ContainerEq(vector<string>{"Hell", ", W", "rld!"}));
	EXPECT_THAT(AOC::tokenize(s1, "l"),
		::testing::ContainerEq(vector<string>{"He", "", "o, Wor", "d!"}));

	string s2 = "A,B;C,D.E";
	EXPECT_THAT(AOC::tokenize(s2, regex("[,:.]")),
		::testing::ContainerEq(vector<string>{"A","B;C","D","E"}));
}

