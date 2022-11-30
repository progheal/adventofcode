#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "util.h"
using namespace std;

template<class T>
void printAll(T v)
{
	for(auto& token : v) cout << "\"" << token << "\" ";
	cout << endl;
}

int main()
{
	string s1 = "Hello, World!";
	printAll(AOC::tokenize(s1, "o"));
	printAll(AOC::tokenize(s1, "l"));

	string s2 = "A,B;C,D.E";
	printAll(AOC::tokenize(s2, regex("[,:.]")));

	return 0;
}

