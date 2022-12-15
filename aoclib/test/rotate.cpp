#include <gtest/gtest.h>
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

    EXPECT_EQ(AOC::rotateCW(input1),
        (vector<string>{
            "upkfaUPKFA",
            "vqlgbVQLGB",
            "wrmhcWRMHC",
            "xsnidXSNID",
            "ytojeYTOJE",
        }));
    EXPECT_EQ(AOC::rotate180(input1),
        (vector<string>{
            "yxwvu", "tsrqp", "onmlk", "jihgf", "edcba",
            "YXWVU", "TSRQP", "ONMLK", "JIHGF", "EDCBA"
        }));
    EXPECT_EQ(AOC::rotateCCW(input1),
        (vector<string>{
            "EJOTYejoty",
            "DINSXdinsx",
            "CHMRWchmrw",
            "BGLQVbglqv",
            "AFKPUafkpu"
        }));
    EXPECT_EQ(AOC::rotateCW(input2),
        (vector<string>{
            "*****",
            "**** ",
            "***  ",
            "**   ",
            "*    "
        }));
    EXPECT_EQ(AOC::rotate180(input2),
        (vector<string>{
            "*****",
            " ****",
            "  ***",
            "   **",
            "    *"
        }));
    EXPECT_EQ(AOC::rotateCCW(input2),
        (vector<string>{
            "    *",
            "   **",
            "  ***",
            " ****",
            "*****"
        }));
}