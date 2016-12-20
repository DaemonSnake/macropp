#include <tuple>
#include <utility>
#include <iostream>
#include <typeinfo>

enum class for_tuple_keys : int {
    continue_ = 0,
    break_
};

template <size_t size>
struct TupleIterator
{
    template <class T, class V, size_t index = 0>
    static auto iterate(T holder, V func) {
        if constexpr (index < size)
         if (auto ret  = func(std::get<index>(holder)); ret != for_tuple_keys::break_)
               return iterate<T, V, index + 1>(holder, func);
        return for_tuple_keys::continue_;
    }
};

#define ftuple_kw(key)                          \
    return for_tuple_keys::key ## _

#define for_tuple(name, args...)                                        \
    {                                                                   \
        auto tuple = args;                                              \
        TupleIterator<std::tuple_size<decltype(tuple)>::value>::iterate(tuple, [&](auto x) \
        [@ ); } @]

int main()
{
    for_tuple(x, std::make_tuple(42, "Hello", 3.14)) {
        std::cout << x << std::endl;
        if constexpr (std::is_same<const char *, decltype(x)>::value)
            ftuple_kw(break);
        else
            ftuple_kw(continue);
    }
}
