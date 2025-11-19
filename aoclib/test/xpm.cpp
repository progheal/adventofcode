#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <string>
#include <vector>
#include "aoc/grid.h"
#include "aoc/xpm_export.h"
using namespace std;
using namespace testing;

TEST(XPMTest, SimpleGenerate)
{
    vector<string> test {
        "123456",
        "654321",
        "123456",
        "654321",
    };
    AOC::Grid g (test);

    ostringstream ss;
    AOC::xpm_export(ss, g, {
        {"1", "#000000"},
        {"2", "#333333"},
        {"3", "#666666"},
        {"4", "#999999"},
        {"5", "#CCCCCC"},
        {"6", "#FFFFFF"},
    });
    std::string out = ss.str();

    EXPECT_EQ(out, 
R"(/* XPM */
static char * xpm[] = {
"6 4 6 1",
"1 c #000000",
"2 c #333333",
"3 c #666666",
"4 c #999999",
"5 c #CCCCCC",
"6 c #FFFFFF",
"123456",
"654321",
"123456",
"654321",
};
)");
}

TEST(XPMTest, ConvertGenerate)
{
    vector<vector<int>> table {
        {10, 20, 30, 40, 50},
        {17, 27, 37, 47, 57},
        {24, 34, 44, 54, 64},
        {31, 41, 51, 61, 71},
    };
    AOC::Grid g (table);

    ostringstream ss;
    AOC::xpm_export(ss, g, {
        {"A ", "#000000"},
        {"B ", "#333333"},
        {"C ", "#666666"},
        {"D ", "#999999"},
        {"E ", "#CCCCCC"},
        {"F ", "#FFFFFF"},
    }, [](int x){return (char)('A' + x % 6) + " "s;});
    std::string out = ss.str();

    EXPECT_EQ(out, 
R"(/* XPM */
static char * xpm[] = {
"5 4 6 2",
"A  c #000000",
"B  c #333333",
"C  c #666666",
"D  c #999999",
"E  c #CCCCCC",
"F  c #FFFFFF",
"E C A E C ",
"F D B F D ",
"A E C A E ",
"B F D B F ",
};
)");
}