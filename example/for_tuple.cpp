#include <tuple>
#include <utility>
#include <iostream>
#include <typeinfo>

enum class for_tuple_keys : int {
    nothing = 0,
    break_,
    //return_ in future
    continue_
};

template <size_t size>
class VarargsIterator
{
    template <size_t index, bool isOk = (index < size)>
    struct Iterator
    {
        static auto turn(auto t, auto func) {
            auto ret = func(std::get<index>(t));
            if (ret == for_tuple_keys::break_)
                return ret;
            return Iterator<index + 1>::turn(t, func);
        }
    };

    template <size_t index>
    struct Iterator<index, false>
    {
        static auto turn(auto t, auto func) {
            return for_tuple_keys::nothing;
        }
    };

public:
    static auto iterate(auto holder, auto func) {
        return Iterator<0>::turn(holder, func);
    }
};

#define ftuple_kw(key)                                                  \
    return for_tuple_keys::key ## _

#define for_tuple(name, args...)                                        \
    { [@FORMAT "\n" @]                                                  \
            VarargsIterator<std::tuple_size<decltype(args)>::value>::iterate \
            (args, [&](auto &name)                                      \
             [@ );%NL                                                   \
             } %NL @]

int main()
{
    for_tuple(x, std::make_tuple(42, "Hello", 3.14))
    {
        if (typeid(x) == typeid(double))
            ftuple_kw(break);
        std::cout << x << std::endl;
    }
}
