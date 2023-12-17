#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include "aoc/grid.h"
using namespace std;
using namespace testing;

MATCHER_P2(CoordMatches, x, y,
    (negation ? "!="s : "="s) + "Coord {" + to_string(x) + "," + to_string(y) + "}")
{
    return A<AOC::Coord>().MatchAndExplain(arg, result_listener)
        && (arg.x == x && arg.y == y);
}

MATCHER_P2(VectorMatches, x, y,
    (negation ? "!="s : "="s) + "Vector {" + to_string(x) + "," + to_string(y) + "}")
{
    return A<AOC::Vector>().MatchAndExplain(arg, result_listener)
        && (arg.x == x && arg.y == y);
}

TEST(GridTest, CoordArithmetic)
{
    AOC::Coord c {3, 5};
    AOC::Vector v {2, 7}, w {8, 4};

    EXPECT_THAT(c + v, CoordMatches(5, 12));
    EXPECT_THAT(c - v, CoordMatches(1, -2));
    EXPECT_THAT(v + w, VectorMatches(10, 11));
    EXPECT_THAT(v - w, VectorMatches(-6, 3));
    EXPECT_THAT(v * 2, VectorMatches(4, 14));
    EXPECT_THAT(3 * w, VectorMatches(24, 12));

    EXPECT_THAT(c + AOC::Vector::NORTHWEST, CoordMatches(2, 4));
    EXPECT_THAT(c + AOC::Vector::NORTH,     CoordMatches(2, 5));
    EXPECT_THAT(c + AOC::Vector::NORTHEAST, CoordMatches(2, 6));
    EXPECT_THAT(c + AOC::Vector::WEST,      CoordMatches(3, 4));
    EXPECT_THAT(c + AOC::Vector::EAST,      CoordMatches(3, 6));
    EXPECT_THAT(c + AOC::Vector::SOUTHWEST, CoordMatches(4, 4));
    EXPECT_THAT(c + AOC::Vector::SOUTH,     CoordMatches(4, 5));
    EXPECT_THAT(c + AOC::Vector::SOUTHEAST, CoordMatches(4, 6));

    vector<AOC::Coord> vc;
    for(auto d : AOC::Vector::VonNeumannNeighborhood)
    {
        vc.push_back(c + d);
    }
    EXPECT_THAT(vc, UnorderedElementsAre(
        CoordMatches(2, 5),
        CoordMatches(3, 4),
        CoordMatches(3, 6),
        CoordMatches(4, 5)
    ));

    vc.clear();
    for(auto d : AOC::Vector::MooreNeighborhood)
    {
        vc.push_back(c + d);
    }
    EXPECT_THAT(vc, UnorderedElementsAre(
        CoordMatches(2, 4),
        CoordMatches(2, 5),
        CoordMatches(2, 6),
        CoordMatches(3, 4),
        CoordMatches(3, 6),
        CoordMatches(4, 4),
        CoordMatches(4, 5),
        CoordMatches(4, 6)
    ));
}

TEST(GridTest, CoordHashes)
{
    std::hash<AOC::Coord> hc;
    const size_t SEP = (1LL << 32);
    EXPECT_EQ(hc(AOC::Coord{0, 0}), 0);
    EXPECT_EQ(hc(AOC::Coord{1, 0}), SEP);
    EXPECT_EQ(hc(AOC::Coord{0, 1}), 1);
    EXPECT_EQ(hc(AOC::Coord{-1, 0}), (size_t)((size_t)(-1) * SEP));
    EXPECT_EQ(hc(AOC::Coord{0, -1}), (size_t)(-1));
    EXPECT_EQ(hc(AOC::Coord{3, 4}), (size_t)(3 * SEP + 4));
    EXPECT_EQ(hc(AOC::Coord{-3, 4}), (size_t)((size_t)(-3) * SEP + 4));
    EXPECT_EQ(hc(AOC::Coord{3, -4}), (size_t)(3 * SEP - 4));
    EXPECT_EQ(hc(AOC::Coord{-3, -4}), (size_t)((size_t)(-3) * SEP - 4));
}

TEST(GridTest, VectorHashes)
{
    std::hash<AOC::Vector> hv;
    EXPECT_EQ(hv(AOC::Vector{0, 0}), 0);
    EXPECT_EQ(hv(AOC::Vector{1, 0}), 1);
    EXPECT_EQ(hv(AOC::Vector{0, 1}), 2);
    EXPECT_EQ(hv(AOC::Vector{-1, 0}), 3);
    EXPECT_EQ(hv(AOC::Vector{0, -1}), 4);
    EXPECT_EQ(hv(AOC::Vector{2, 0}), 5);
    EXPECT_EQ(hv(AOC::Vector{1, 1}), 6);
    EXPECT_EQ(hv(AOC::Vector{0, 2}), 7);
    EXPECT_EQ(hv(AOC::Vector{-1, 1}), 8);
    EXPECT_EQ(hv(AOC::Vector{-2, 0}), 9);
    EXPECT_EQ(hv(AOC::Vector{-1, -1}), 10);
    EXPECT_EQ(hv(AOC::Vector{0, -2}), 11);
    EXPECT_EQ(hv(AOC::Vector{1, -1}), 12);
    EXPECT_EQ(hv(AOC::Vector{3, 0}), 13);
    EXPECT_EQ(hv(AOC::Vector{2, 1}), 14);
    EXPECT_EQ(hv(AOC::Vector{1, 2}), 15);
    EXPECT_EQ(hv(AOC::Vector{0, 3}), 16);
    EXPECT_EQ(hv(AOC::Vector{-1, 2}), 17);
    EXPECT_EQ(hv(AOC::Vector{-2, 1}), 18);
    EXPECT_EQ(hv(AOC::Vector{-3, 0}), 19);
    EXPECT_EQ(hv(AOC::Vector{-2, -1}), 20);
    EXPECT_EQ(hv(AOC::Vector{-1, -2}), 21);
    EXPECT_EQ(hv(AOC::Vector{0, -3}), 22);
    EXPECT_EQ(hv(AOC::Vector{1, -2}), 23);
    EXPECT_EQ(hv(AOC::Vector{2, -1}), 24);
}

TEST(GridTest, GridRead)
{
    vector<string> maze {
        "ABCDEFGHI",
        "JKLMNOPQR",
        "STUVWXYZ@",
        "abcdefghi",
        "jklmnopqr",
        "stuvwxyz$",
        "123456789"
    };

    AOC::Grid g(maze, '#');
    AOC::Coord c {3, 4};

    EXPECT_EQ(g[0][0], 'A');
    EXPECT_EQ(g[2][3], 'V');
    EXPECT_EQ(g[c], 'e');
    EXPECT_EQ(g[6][8], '9');
    EXPECT_EQ(g[-4][3], '#');
    EXPECT_EQ(g[8][3], '#');
    EXPECT_EQ(g[3][-5], '#');
    EXPECT_EQ(g[5][12], '#');
    EXPECT_EQ(g[-6][-4], '#');
    EXPECT_EQ(g[-2][11], '#');
    EXPECT_EQ(g[9][-3], '#');
    EXPECT_EQ(g[13][14], '#');

    vector<vector<int>> maze2;
    int value = 0;
    for(int i = 0; i < 20; i++)
    {
        maze2.push_back({});
        generate_n(back_inserter(maze2.back()), 30,
            [&](){return ++value;});
    }
    AOC::Grid g2 (maze2);
    EXPECT_EQ(g2[0][0], 1);
    EXPECT_EQ(g2[0][9], 10);
    EXPECT_EQ(g2[1][0], 31);
    EXPECT_EQ(g2[5][5], 156);
    EXPECT_EQ(g2[19][29], 600);
    EXPECT_EQ(g2[-4][10], 0);
    EXPECT_EQ(g2[25][20], 0);
    EXPECT_EQ(g2[8][-7], 0);
    EXPECT_EQ(g2[13][44], 0);
    EXPECT_EQ(g2[-5][-8], 0);
    EXPECT_EQ(g2[-2][37], 0);
    EXPECT_EQ(g2[22][-1], 0);
    EXPECT_EQ(g2[30][49], 0);

    double m3[5][3] = {
        {0.25, 0.5, 0.75},
        {0.0625, 0.125, 0.1875},
        {0.3125, 0.375, 0.4375},
        {0.5625, 0.625, 0.6875},
        {0.8125, 0.875, 0.9375}
    };
    AOC::Grid g3(m3, -1);
    EXPECT_EQ(g3[0][0], 0.25);
    EXPECT_EQ(g3[2][1], 0.375);
    EXPECT_EQ(g3[4][2], 0.9375);
    EXPECT_EQ(g3[-1][1], -1);
    EXPECT_EQ(g3[7][2], -1);
    EXPECT_EQ(g3[1][-2], -1);
    EXPECT_EQ(g3[3][6], -1);
    EXPECT_EQ(g3[-4][-7], -1);
    EXPECT_EQ(g3[-3][8], -1);
    EXPECT_EQ(g3[6][-5], -1);
    EXPECT_EQ(g3[9][10], -1);
}

TEST(GridTest, GridWrite)
{
    vector<string> maze {
        "ABCDEFGHI",
        "JKLMNOPQR",
        "STUVWXYZ@",
        "abcdefghi",
        "jklmnopqr",
        "stuvwxyz$",
        "123456789"
    };

    AOC::Grid g(maze, '#');
    
    
}
