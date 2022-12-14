#pragma once

#include <iosfwd>

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
    static const Vector VonNeumannNeighborhood[4];
    static const Vector MooreNeighborhood[8];

    constexpr Vector& operator += (const Vector& v) {x += v.x; y += v.y; return *this;}
    constexpr Vector& operator -= (const Vector& v) {x -= v.x; y -= v.y; return *this;}
    constexpr Vector& operator *= (int scalar) {x *= scalar; y *= scalar; return *this;}
    // Vector& operator /= (int scalar) {x /= scalar; y /= scalar; return *this;}

    constexpr Vector CW90() {return Vector{y, -x};}
    constexpr Vector CCW90() {return Vector{-y, x};}
    constexpr Vector Rotate180() {return Vector{-x, -y};}
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

inline constexpr Vector operator + (Vector v1, const Vector& v2) {return v1 += v2;}
inline constexpr Vector operator - (Vector v1, const Vector& v2) {return v1 -= v2;}
inline constexpr Vector operator * (Vector v, int scalar) {return v *= scalar;}
inline constexpr Vector operator * (int scalar, Vector v) {return v *= scalar;}
// inline constexpr Vector operator / (Vector v, int scalar) {return v /= scalar;}

std::ostream& operator << (std::ostream& out, const Vector& v)
{
    return out << '<' << v.x << ',' << v.y << '>'; 
}

// 座標
struct Coord
{
    int x, y;

    constexpr Coord(int x = 0, int y = 0) : x(x), y(y) {}

    constexpr Coord& operator += (const Vector& v) {x += v.x; y += v.y; return *this;}
    constexpr Coord& operator -= (const Vector& v) {x -= v.x; y -= v.y; return *this;}
    constexpr Vector operator - (const Coord& rhs) const {return {x - rhs.x, y - rhs.y};}
};

inline constexpr Coord operator + (Coord c, const Vector& v) {return c += v;}
inline constexpr Coord operator + (const Vector& v, Coord c) {return c += v;} 
inline constexpr Coord operator - (Coord c, const Vector& v) {return c -= v;}

std::ostream& operator << (std::ostream& out, const Coord& c)
{
    return out << '(' << c.x << ',' << c.y << ')'; 
}

struct CoordOrder
{
    constexpr bool operator ()(const Coord& c1, const Coord& c2) const
    {
        return c1.x == c2.x ? c1.y < c2.y : c1.x < c2.x;
    }
};

template<class TwoDContainer>
class Grid
{
    class RowHelper
    {
        Grid* parent;
        size_t row;
    public:
        RowHelper(Grid* p, size_t r) : parent(p), row(r) {}
        auto& operator[](size_t index)
        {
            return parent->at(row, index);
        }
        auto& at(size_t index)
        {
            return parent->at(row, index);
        }
    };
    class ConstRowHelper
    {
        const Grid* parent;
        size_t row;
    public:
        ConstRowHelper(const Grid* p, size_t r) : parent(p), row(r) {}
        const auto& operator[](size_t index)
        {
            return parent->at(row, index);
        }
        const auto& at(size_t index)
        {
            return parent->at(row, index);
        }
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
    RowHelper operator[](size_t index)
    {
        return RowHelper(this, index);
    }
    RowHelper at(size_t index)
    {
        return RowHelper(this, index);
    }
    ConstRowHelper operator [](size_t index) const
    {
        return ConstRowHelper(this, index);
    }
    ConstRowHelper at(size_t index) const
    {
        return ConstRowHelper(this, index);
    }
    Element_t& operator[](const Coord& c)
    {
        return at(c.x, c.y);
    }
    const Element_t& operator[](const Coord& c) const
    {
        return at(c.x, c.y);
    }
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
    size_t width() const {return w;}
    size_t height() const {return h;}
private:
    TwoDContainer& container;
    mutable Element_t oob;
    mutable Element_t external_oob;
    size_t w, h;
};

}