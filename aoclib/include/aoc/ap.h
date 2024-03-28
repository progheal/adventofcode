
#include <cstdint>
#include <initializer_list>
#include <iosfwd>
#include <type_traits>

namespace AOC
{

class AP
{
    int64_t diff;
    int64_t init;
public:
    AP(int64_t d = 1, int64_t a = 0) : diff(d), init(a) {}
    int64_t min() const {return init;}
    int64_t mod() const {return diff;}
    int64_t mathrem() const {return init >= 0 ? init % diff : (init % diff + diff) % diff;}
    bool has(int64_t value) const {return diff > 0 && value >= init && (value - init) % diff == 0;}
    bool has(const AP& sub) const
    {
        return diff > 0 && (sub.diff == 0 || sub.diff % diff == 0 && has(sub.init));
    }
    bool isEmpty() const { return diff == 0; }
    bool operator == (const AP& rhs) const {return diff == rhs.diff && init == rhs.init;}
    AP& operator *= (int64_t m) {diff *= m; init *= m; return *this;}
    AP operator * (int64_t m) {return AP(*this) *= m;}
    AP& operator += (int64_t d) {init += d; return *this;}
    AP operator + (int64_t d) {return AP(*this) += d;}
    static AP empty() { return AP(0, 1); }
    friend AP APIntersection(const AP& ap1, const AP& ap2);
    friend std::ostream& operator << (std::ostream&, const AP&);
};

AP APIntersection(const std::initializer_list<AP>&);
AP APIntersection(const AP& ap1, const AP& ap2);
template<class... T>
inline
typename std::enable_if<std::conjunction_v<std::is_same<AP, T>...>, AP>::type
APIntersection(const AP& ap1, const AP& ap2, const AP& ap3, const T& ...rest)
{
    return APIntersection(APIntersection(ap1, ap2), ap3, rest...);
}

}