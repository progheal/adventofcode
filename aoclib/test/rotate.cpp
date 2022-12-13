#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>
#include "aoc/util.h"
using namespace std;

TEST(UtilTest, Rotate)
{
    vector<string> input1 = {
        "ABCDE",
        "FGHIJ",
        "KLMNO",
        "PQRST",
        "UVWXY",
        "abcde",
        "fghij",
        "klmno",
        "pqrst",
        "uvwxy"
    };

    vector<string> input2 = {
        "*",
        "**",
        "***",
        "****",
        "*****"
    };

    EXPECT_THAT(AOC::rotateCW(input1),
        ::testing::ContainerEq(vector<string>{
            "upkfaUPKFA",
            "vqlgbVQLGB",
            "wrmhcWRMHC",
            "xsnidXSNID",
            "ytojeYTOJE",
        }));
    EXPECT_THAT(AOC::rotate180(input1),
        ::testing::ContainerEq(vector<string>{
            "yxwvu", "tsrqp", "onmlk", "jihgf", "edcba",
            "YXWVU", "TSRQP", "ONMLK", "JIHGF", "EDCBA"
        }));
    EXPECT_THAT(AOC::rotateCCW(input1),
        ::testing::ContainerEq(vector<string>{
            "EJOTYejoty",
            "DINSXdinsx",
            "CHMRWchmrw",
            "BGLQVbglqv",
            "AFKPUafkpu"
        }));
    EXPECT_THAT(AOC::rotateCW(input2),
        ::testing::ContainerEq(vector<string>{
            "*****",
            "**** ",
            "***  ",
            "**   ",
            "*    "
        }));
    EXPECT_THAT(AOC::rotate180(input2),
        ::testing::ContainerEq(vector<string>{
            "*****",
            " ****",
            "  ***",
            "   **",
            "    *"
        }));
    EXPECT_THAT(AOC::rotateCCW(input2),
        ::testing::ContainerEq(vector<string>{
            "    *",
            "   **",
            "  ***",
            " ****",
            "*****"
        }));
}