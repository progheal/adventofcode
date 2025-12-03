#include <iostream>
#include <string>
#include "aoc/aoc.h"
using namespace std;

auto find_first_max_in(string_view sv)
{
    auto m = sv.begin(), n = m;
    while(++n != sv.end())
    {
        if(*n > *m) m = n;
    }
    return m;
}

string maxjolt(string_view battery, int count)
{
    auto p = find_first_max_in(battery);
    if(count == 1)
    {
        return string{*p};
    }
    else
    {
        auto toEnd = battery.end() - p;
        if(toEnd < count)
            return maxjolt({battery.begin(), p}, count - toEnd) + string(p, battery.end());
        else
            return (*p) + maxjolt({p+1, battery.end()}, count-1);
    }
}

int main(int argc, char* argv[])
{
    bool partb = AOC::PartB(argv[0]);
    string line;
    int64_t jolts = 0;
    while(getline(cin, line))
    {
        string j = maxjolt(line, partb ? 12 : 2);
        cerr << j << "\n";
        jolts += stoll(j);
    }
    cout << jolts << "\n";
}