#include <list>
#include <stdio.h>

#define foreach(name, expr...)                                          \
    {                                                                   \
        auto &&x = expr;                                                \
        for (auto it = x.begin(); it != x.end(); ++it)                  \
        {                                                               \
            auto name = *it; [@ }} @]

int main()
{
    foreach(x, std::list<int>{42, 34, 5})
    {
        printf("%d\n", x);
    }
}
