#pragma once

#include <string>
#include <unordered_map>

namespace AOC
{

namespace detail
{

template<class Callable, class R, class A>
class Cache1
{
    Callable func;
    mutable std::unordered_map<A, R> cache;
public:
    Cache1(Callable f): func(f) {}
    R operator() (const A& arg) const
    {
        auto p = cache.find(arg);
        if(p != cache.end()) return p->second;
        return cache[arg] = func(arg);
    }
};

template<class Callable, class R, class A1, class A2>
class Cache2
{
    Callable func;
    mutable std::unordered_map<A1, std::unordered_map<A2, R>> cache;
public:
    Cache2(Callable f): func(f) {}
    R operator() (const A1& a1, const A2& a2) const
    {
        auto p = cache.find(a1);
        if(p != cache.end())
        {
            auto q = p->second.find(a2);
            if(q != p->second.end()) return q->second;
        }
        return cache[a1][a2] = func(a1, a2);
    }
};

template<class Callable, class R, class... Args>
class StringKeyCache
{
    Callable func;
    mutable std::unordered_map<std::string, R> cache;
public:
    StringKeyCache(Callable f): func(f) {}
    R operator() (const Args&... args) const
    {
        using std::to_string;
        std::string cacheKey = ((to_string(args) + "\xff") + ...);
        auto p = cache.find(cacheKey);
        if(p != cache.end()) return p->second;
        return cache[cacheKey] = func(args...);
    }
};

template<class Callable>
class RecursiveCache
{
    Callable func;
public:
    RecursiveCache(Callable f): func(f) {}
    template<class A>
    auto operator() (const A& arg) const {return operator()(*this, arg);}
    template<class A1, class A2, typename = std::enable_if_t<!std::is_same_v<A1, RecursiveCache>>>
    auto operator() (const A1& a1, const A2& a2) const {return operator()(*this, a1, a2);}
    template<class A>
    auto operator() (const RecursiveCache&, const A& arg) const
    -> decltype(func(func, arg))
    {
        typedef decltype(func(func, arg)) R;
        static std::unordered_map<A, R> cache;
        auto p = cache.find(arg);
        if(p != cache.end()) return p->second;
        return cache[arg] = func(*this, arg);
    }
    template<class A1, class A2>
    auto operator() (const RecursiveCache&, const A1& a1, const A2& a2) const
    -> decltype(func(func, a1, a2))
    {
        typedef decltype(func(func, a1, a2)) R;
        static std::unordered_map<A1, std::unordered_map<A2, R>> cache;
        auto p = cache.find(a1);
        if(p != cache.end())
        {
            auto q = p->second.find(a2);
            if(q != p->second.end()) return q->second;
        }
        return cache[a1][a2] = func(*this, a1, a2);
    }
};

template<class R, class A>
Cache1<R(*)(A), R, A> cache_helper(R (*)(A));
template<class R, class A1, class A2>
Cache2<R(*)(A1,A2), R, A1, A2> cache_helper(R (*)(A1,A2));
template<class Obj, class R, class A>
Cache1<Obj, R, A> cache_helper_obj(Obj obj, R (Obj::*)(A) const);
template<class Obj, class R, class A1, class A2>
Cache2<Obj, R, A1, A2> cache_helper_obj(Obj obj, R (Obj::*)(A1,A2) const);
template<class R, class... Args>
StringKeyCache<R(*)(Args...), R, Args...> stringkey_cache_helper(R (*)(Args...));

template <class T, class = void>
struct has_untemplated_call : public std::false_type {};

template <class T>
struct has_untemplated_call<T,
    std::void_t<decltype(&T::operator())>
> : public std::true_type {};

} // namespace AOC::detail

template<class FObj>
auto cache(const FObj& fobj, std::enable_if_t<
    std::is_pointer_v<FObj> && std::is_function_v<std::remove_pointer<FObj>> ||
    std::is_function_v<FObj>,
    void *> = nullptr)
{
    return decltype(detail::cache_helper(fobj)){fobj};
}

template<class FObj>
auto cache(const FObj& fobj, std::enable_if_t<
    detail::has_untemplated_call<FObj>::value,
    void *> = nullptr)
{
    return decltype(detail::cache_helper_obj(fobj, &FObj::operator())){fobj};
}

template<class Func>
auto stringkey_cache(const Func& fobj)
{
    return decltype(detail::stringkey_cache_helper(fobj)){fobj};
}

template<class FObj>
auto cache_recursive(const FObj& fobj)
{
    return detail::RecursiveCache<FObj>(fobj);
}

} // namespace AOC