#pragma once

#include <algorithm>
#include <cmath>
#include <iosfwd>
#include <string>
#include <type_traits>
#if __cplusplus >= 202002L
#include <format>
#endif

namespace AOC
{

// 二維座標結構
// 由於語意問題，將「座標」及「座標差=向量」兩個概念分開

// 向量，表示座標差
struct Vector
{
    int x, y;

    constexpr Vector(int x = 0, int y = 0) : x(x), y(y) {}

    static const Vector NORTHWEST, NORTH, NORTHEAST,
                             WEST,             EAST,
                        SOUTHWEST, SOUTH, SOUTHEAST;
    // von-Neumann neighborhood: 4 cardinal direction
    static const Vector VonNeumannNeighborhood[4];
    // Moore neighborhood: 8 direction around
    static const Vector MooreNeighborhood[8];
    // knight "neighborhood": 8 chess knight moves
    static const Vector KnightNeighborhood[8];

    constexpr Vector& operator += (const Vector& v) {x += v.x; y += v.y; return *this;}
    constexpr Vector& operator -= (const Vector& v) {x -= v.x; y -= v.y; return *this;}
    constexpr Vector& operator *= (int scalar) {x *= scalar; y *= scalar; return *this;}
    // Vector& operator /= (int scalar) {x /= scalar; y /= scalar; return *this;}

    bool operator == (const Vector& rhs) const {return x == rhs.x && y == rhs.y;}
    bool operator != (const Vector& rhs) const {return x != rhs.x || y != rhs.y;}

    constexpr Vector CW90() const {return Vector{y, -x};}
    constexpr Vector CCW90() const {return Vector{-y, x};}
    constexpr Vector Rotate180() const {return Vector{-x, -y};}
    constexpr Vector operator - () const {return Vector{-x, -y};}

    int manhattanDistance() const {return std::abs(x) + std::abs(y);}
    int64_t euclideanDistanceSquare() const {return (int64_t)(x)*x + (int64_t)(y)*y;}
    double euclideanDistance() const {return std::hypot(x, y);}
    int chebyshevDistance() const {return std::max(std::abs(x), std::abs(y));}

    size_t hash() const
    {
        if(x == 0 && y == 0) return 0;
        int s = (x >= 0 ? x : -x) + (y >= 0 ? y : -y);
        if(s == 1) return y == 0 ? 2 - x : 3 - y;
        int b = s * (s-1) * 2 + 1;
        if(x > 0 && y >= 0) return b + y;
        else if(x <= 0 && y > 0) return b + s - x;
        else if(x < 0 && y <= 0) return b + s*2 - y;
        else return b + s*3 + x;
    }
};

constexpr Vector Vector::NORTHWEST {-1, -1};
constexpr Vector Vector::NORTH     {-1, 0};
constexpr Vector Vector::NORTHEAST {-1, 1};
constexpr Vector Vector::WEST      {0, -1};
constexpr Vector Vector::EAST      {0, 1};
constexpr Vector Vector::SOUTHWEST {1, -1};
constexpr Vector Vector::SOUTH     {1, 0};
constexpr Vector Vector::SOUTHEAST {1, 1};

constexpr Vector Vector::VonNeumannNeighborhood[4] = {
    Vector::NORTH,
    Vector::EAST,
    Vector::SOUTH,
    Vector::WEST
};

constexpr Vector Vector::MooreNeighborhood[8] = {
    Vector::NORTH,
    Vector::NORTHEAST,
    Vector::EAST,
    Vector::SOUTHEAST,
    Vector::SOUTH,
    Vector::SOUTHWEST,
    Vector::WEST,
    Vector::NORTHWEST
};

constexpr Vector Vector::KnightNeighborhood[8] = {
    {2, 1},
    {1, 2},
    {-1, 2},
    {-2, 1},
    {-2, -1},
    {-1, -2},
    {1, -2},
    {2, -1},
};

inline constexpr Vector operator + (Vector v1, const Vector& v2) {return v1 += v2;}
inline constexpr Vector operator - (Vector v1, const Vector& v2) {return v1 -= v2;}
inline constexpr Vector operator * (Vector v, int scalar) {return v *= scalar;}
inline constexpr Vector operator * (int scalar, Vector v) {return v *= scalar;}
// inline constexpr Vector operator / (Vector v, int scalar) {return v /= scalar;}

inline std::string to_string(const Vector& v)
{
    return "<" + std::to_string(v.x) + "," + std::to_string(v.y) + ">";
}

inline std::ostream& operator << (std::ostream& out, const Vector& v)
{
    return out << to_string(v); 
}

// 座標
struct Coord
{
    int x, y;

    constexpr Coord(int x = 0, int y = 0) : x(x), y(y) {}

    constexpr Coord& operator += (const Vector& v) {x += v.x; y += v.y; return *this;}
    constexpr Coord& operator -= (const Vector& v) {x -= v.x; y -= v.y; return *this;}
    constexpr Vector operator - (const Coord& rhs) const {return {x - rhs.x, y - rhs.y};}

    bool operator == (const Coord& rhs) const {return x == rhs.x && y == rhs.y;}
    bool operator != (const Coord& rhs) const {return x != rhs.x || y != rhs.y;}

    size_t hash() const
    {
        return (size_t)(x * (1LL << 32) + y);
    }
};

inline constexpr Coord operator + (Coord c, const Vector& v) {return c += v;}
inline constexpr Coord operator + (const Vector& v, Coord c) {return c += v;} 
inline constexpr Coord operator - (Coord c, const Vector& v) {return c -= v;}

// 轉成 Excel 格式座標 (例：(0,0) => "A1"、(1,3) => "D2")
// 直欄字母順序為 A, B, ..., Z, AA, AB, ..., AZ, BA, ..., BZ, ..., ZZ, AAA, ...
// 負座標直欄前置 x，橫列前置 _ (例：（-1,-4) => "xD_1")
inline std::string to_excel_string(const Coord& c)
{
    std::string s = "";
    int y = c.y >= 0 ? c.y + 1 : -c.y;
    while(y > 0)
    {
        int r = (y - 1) % 26;
        s = (char)('A' + r) + s;
        y = (y - r) / 26;
    }
    if(c.y < 0) s = 'x' + s;
    if(c.x < 0)
        s += '_' + std::to_string(-c.x);
    else
        s += std::to_string(c.x + 1);
    return s;
}

inline std::string to_string(const Coord& c)
{
    return "(" + std::to_string(c.x) + "," + std::to_string(c.y) + ")";
}

inline std::ostream& operator << (std::ostream& out, const Coord& c)
{
    return out << to_string(c);
}

struct CoordOrder
{
    constexpr bool operator ()(const Coord& c1, const Coord& c2) const
    {
        return std::tie(c1.x, c1.y) < std::tie(c2.x, c2.y);
    }
};

template<class TwoDContainer = std::vector<std::string>>
class Grid
{
    template<bool Modifiable>
    class RowHelper
    {
        typedef std::conditional_t<Modifiable, Grid*, const Grid*> pGrid_t;
        pGrid_t parent;
        size_t row;
    public:
        RowHelper(pGrid_t p, size_t r) : parent(p), row(r) {}
        auto& operator[](size_t index) const {return parent->at(row, index);}
        auto& at(size_t index) const {return parent->at(row, index);}
    };
public:
    typedef std::remove_reference_t<
            decltype(std::declval<TwoDContainer>()[0][0])
        > Element_t;

    Grid(TwoDContainer& source, Element_t def = {})
        : container(source), oob(def),
          h(std::size(source)), w(std::size(source[0]))
        {}
    void setOOB(Element_t def) const
    {
        oob = def;
    }
    bool inBound(const Coord& c) const
    {
        return (size_t)(c.x) < h && (size_t)(c.y) < w;
    }
    const auto operator[](size_t index) {return RowHelper<true>(this, index);}
    const auto at(size_t index) {return RowHelper<true>(this, index);}
    const auto operator [](size_t index) const {return RowHelper<false>(this, index);}
    const auto at(size_t index) const {return RowHelper<false>(this, index);}
    Element_t& operator[](const Coord& c) {return at(c.x, c.y);}
    const Element_t& operator[](const Coord& c) const {return at(c.x, c.y);}
    Element_t& at(size_t x, size_t y)
    {
        if(x >= h || y >= w)
        {
            external_oob = oob;
            return external_oob;
        }
        else
            return container[x][y];
    }
    const Element_t& at(size_t x, size_t y) const
    {
        if(x >= h || y >= w)
            return oob;
        else
            return container[x][y];
    }
    template<class Func>
    Coord find_if(Func f) const
    {
        for(auto p = begin(); p != end(); ++p)
            if(f(*p))
                return p;
        return Coord{-1, -1};
    }
    Coord find_first_of(Element_t elem) const
    {
        return find_if([elem](Element_t e){return e == elem;});
    }
    size_t width() const {return w;}
    size_t height() const {return h;}
    auto begin() {return PositionIterator<true>(this, {0, 0});}
    auto end() {return PositionIterator<true>(this, {-1, -1});}
    auto begin() const {return PositionIterator<false>(this, {0, 0});}
    auto end() const {return PositionIterator<false>(this, {-1, -1});}
private:
    TwoDContainer& container;
    mutable Element_t oob;
    mutable Element_t external_oob;
    size_t w, h;
    template<bool Modifiable>
    class PositionIterator
    {
        typedef std::conditional_t<Modifiable, Grid*, const Grid*> pGrid_t;
        pGrid_t parent;
        AOC::Coord pos;
        void inc()
        {
            pos.y++;
            if(pos.y >= parent->width()) {pos.x++; pos.y = 0;}
        }
    public:
        typedef std::ptrdiff_t difference_type;
        typedef std::conditional_t<Modifiable, Grid::Element_t, const Grid::Element_t> value_type;
        typedef std::conditional_t<Modifiable, Grid::Element_t*, const Grid::Element_t*> pointer;
        typedef std::conditional_t<Modifiable, Grid::Element_t&, const Grid::Element_t&> reference;
        typedef std::forward_iterator_tag iterator_category;

        PositionIterator(pGrid_t p, size_t x, size_t y): parent(p), pos(x, y) {}
        PositionIterator(pGrid_t p, const AOC::Coord& c): parent(p), pos(c) {}
        operator bool () const {return parent->inBound(pos);}
        operator AOC::Coord() const {return pos;}
        AOC::Coord position() const {return pos;}
        bool operator == (const PositionIterator& it) const
        {
            if(!*this && !it) return true;
            if(!*this || !it) return false;
            return this->pos == it.pos;
        }
        bool operator != (const PositionIterator& it) const {return !(operator ==(it));}
        auto& operator * () const {return parent->at(pos.x, pos.y);}
        auto& operator -> () const {return &parent->at(pos.x, pos.y);}
        PositionIterator& operator ++ () { inc(); return *this; }
        PositionIterator operator ++ (int) { auto ret = *this; inc(); return ret; }
    };
};

template<class T>
std::ostream& operator << (std::ostream& out, const Grid<T>& g)
{
    using namespace std::string_literals;
    for(size_t i = 0; i < g.height(); i++)
    {
        for(size_t j = 0; j < g.width(); j++)
        {
            out << g[i][j];
            if constexpr (!std::is_same_v<char, typename Grid<T>::Element_t>)
            {
                out << " "s;
            }
        }
        out << "\n"s;
    }
    return out;
}

// 移動物體，有位置及方向
struct Mob
{
    AOC::Coord pos;
    AOC::Vector dir;
    Mob(const AOC::Coord& p, const AOC::Vector& d): pos(p), dir(d) {}
    operator AOC::Coord() const {return pos;}
    operator AOC::Vector() const {return dir;}
    bool operator == (const Mob& mob) const {return pos == mob.pos && dir == mob.dir;}
    bool operator != (const Mob& mob) const {return !(operator ==(mob));}
    void move(int time = 1) {pos += dir * time;}
    void wrap(int Xmin, int Xmax, int Ymin, int Ymax)
    {
        pos.x = (pos.x - Xmin) % (Xmax - Xmin + 1) + Xmin;
        if(pos.x < Xmin) pos.x += Xmax - Xmin + 1;
        pos.y = (pos.y - Ymin) % (Ymax - Ymin + 1) + Ymin;
        if(pos.y < Ymin) pos.y += Ymax - Ymin + 1;
    }
    AOC::Coord ahead() {return pos + dir;}
    void CW90() {dir = dir.CW90();}
    void CCW90() {dir = dir.CCW90();}
    void Rotate180() {dir = dir.Rotate180();}
    size_t hash() const
    {
        return pos.hash() * 9 + dir.hash();
    }
};

inline std::string to_string(const Mob& m)
{
    return to_string(m.pos) + to_string(m.dir);
}

inline std::ostream& operator << (std::ostream& out, const Mob& m)
{
    return out << to_string(m);
}

} // namespace AOC

// std::hash
template<> struct std::hash<AOC::Coord>
{ size_t operator ()(const AOC::Coord& c) const {return c.hash();} };
template<> struct std::hash<AOC::Vector>
{ size_t operator ()(const AOC::Vector& v) const {return v.hash();} };
template<> struct std::hash<AOC::Mob>
{ size_t operator ()(const AOC::Mob& m) const {return m.hash();} };

#if __cplusplus >= 202002L
// std::formatter
template<> struct std::formatter<AOC::Coord> : std::formatter<string_view>
{
    bool isExcel = false;
    // {:x} 以 Excel 格式印出座標；若要其他字串格式，x 需置於字串格式之前
    constexpr auto parse(auto& ctx)
    {
        if(*ctx.begin() == 'x' || *ctx.begin() == 'X')
        {
            isExcel = true;
            ctx.advance_to(next(ctx.begin()));
        }
        return std::formatter<string_view>::parse(ctx);
    }
    auto format(const AOC::Coord& c, auto& ctx) const
    {
        if(isExcel)
            return std::formatter<string_view>::format(to_excel_string(c), ctx);
        else
            return std::formatter<string_view>::format(to_string(c), ctx);
    }
};
template<> struct std::formatter<AOC::Vector> : std::formatter<string_view>
{
    auto format(const AOC::Vector& v, auto& ctx) const
    {
        return std::formatter<string_view>::format(to_string(v), ctx);
    }
    // parse 繼承 std::formatter<string_view>
};
template<> struct std::formatter<AOC::Mob> : std::formatter<string_view>
{
    auto format(const AOC::Mob& m, auto& ctx) const
    {
        return std::formatter<string_view>::format(to_string(m), ctx);
    }
    // parse 繼承 std::formatter<string_view>
};
#endif
