#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "aoc/aoc.h"
#include "aoc/grid.h"
#include "aoc/bfs.h"
using namespace std;

struct State
{
	static AOC::Grid<vector<string>> *pgr;
	static int path;

	AOC::Coord pos;
	typedef int cost_t;
	bool isFinish() const {return false;}
	vector<pair<State, int>> nextMoves() const;
	int id() const { return pos.x * pgr->width() + pos.y; }
	void visit(int cost) const
	{
		if((*pgr)[pos] != '.') path = max(path, cost);
	}
};

AOC::Grid<vector<string>> *State::pgr = nullptr;
int State::path = 0;

inline bool haveNorth(char c) { return c == 'S' || c == '|' || c == 'L' || c == 'J'; }
inline bool haveSouth(char c) { return c == 'S' || c == '|' || c == 'F' || c == '7'; }
inline bool haveEast(char c)  { return c == 'S' || c == '-' || c == 'L' || c == 'F'; }
inline bool haveWest(char c)  { return c == 'S' || c == '-' || c == 'J' || c == '7'; }

vector<pair<State, int>> State::nextMoves() const
{
	vector<pair<State, int>> ret;
	char p = (*pgr)[pos];
	if(haveNorth(p) && haveSouth((*pgr)[pos + AOC::Vector::NORTH]))
		ret.push_back(pair{State{pos + AOC::Vector::NORTH}, 1});
	if(haveSouth(p) && haveNorth((*pgr)[pos + AOC::Vector::SOUTH]))
		ret.push_back(pair{State{pos + AOC::Vector::SOUTH}, 1});
	if(haveEast(p) && haveWest((*pgr)[pos + AOC::Vector::EAST]))
		ret.push_back(pair{State{pos + AOC::Vector::EAST}, 1});
	if(haveWest(p) && haveEast((*pgr)[pos + AOC::Vector::WEST]))
		ret.push_back(pair{State{pos + AOC::Vector::WEST}, 1});
	return ret;
}

int main(int argc, char* argv[])
{
	bool partb = AOC::PartB(argv[0]);

	vector<string> g = AOC::readPicture(cin);
	AOC::Grid gr (g, '.');
	State::pgr = &gr;
	State::path = 0;

	int y;
	int x = find_if(begin(g), end(g), [&](const string& s)
	{ return (y = s.find('S')) != string::npos; }) - begin(g);

	AOC::Coord startPos {x, y};
	int sp = 0;
	if(haveSouth(gr[startPos + AOC::Vector::NORTH]))
		sp += 1;
	if(haveWest(gr[startPos + AOC::Vector::EAST]))
		sp += 2;
	if(haveNorth(gr[startPos + AOC::Vector::SOUTH]))
		sp += 4;
	if(haveEast(gr[startPos + AOC::Vector::WEST]))
		sp += 8;
	if(sp == 3) gr[startPos] = 'L';
	else if(sp == 6) gr[startPos] = 'F';
	else if(sp == 12) gr[startPos] = '7';
	else if(sp == 9) gr[startPos] = 'J';
	else if(sp == 5) gr[startPos] = '|';
	else if(sp == 10) gr[startPos] = '-';
	else cerr << "Cannot determine S pipe! sp = " << sp << endl;

	if(!partb)
	{
		AOC::BFS(State{startPos});
		cout << State::path << endl;
	}
	else
	{
		vector<AOC::Coord> points;
		AOC::Vector from;
		AOC::Coord curr = startPos;
		if(sp & 1) from = AOC::Vector::NORTH;
		else if(sp & 2) from = AOC::Vector::EAST;
		else from = AOC::Vector::SOUTH;

		int twoArea = 0;
		int pickEdge = 0;
		do
		{
			//cerr << "Tracing at " << curr << ": " << gr[curr] << endl;
			points.push_back(curr);

			AOC::Vector vsum;
			switch(gr[curr])
			{
			case '|': case '-':
				vsum = AOC::Vector(); break;
			case 'L':
				vsum = AOC::Vector::NORTHEAST; break;
			case 'F':
				vsum = AOC::Vector::SOUTHEAST; break;
			case '7':
				vsum = AOC::Vector::SOUTHWEST; break;
			case 'J':
				vsum = AOC::Vector::NORTHWEST; break;
			default:
				cerr << "Error tracing!" << endl;
				break;
			}
			auto exit = vsum - from;
			auto next = curr + exit;
			from = exit.Rotate180();

			// Edge point for pick
			++pickEdge;
			// Shoelace formula
			twoArea += curr.x * next.y - curr.y * next.x;

			curr = next;
		} while(curr != startPos);
		//cerr << "Edge point for pick: " << pickEdge << endl;
		if(twoArea < 0) twoArea = -twoArea;
		//cerr << "Area: " << twoArea << "/2" << endl;

		// Pick's theorem: area = i + p/2 - 1
		// so i = (2 * area - p) / 2 + 1
		int internal = (twoArea - pickEdge) / 2 + 1;
		cout << internal << endl;
	}

	return 0;
}

