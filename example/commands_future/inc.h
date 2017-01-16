typedef unsigned long size_t;
typedef enum {
    false = 0,
    true
} bool;

#define MANGLE(x, y) __MANGLE_1(x, y)
#define CMANGLE(x) MANGLE(COMMAND, x)
#define __MANGLE_1(x, y) __MANGLE_2(x, y)
#define __MANGLE_2(x, y) x ## __ ## y
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define NEW_SUBCOMMAND(name, sub) bool MANGLE(name, sub ## __ ## subcommand)()
#define __TO_STR_2(name) #name
#define __TO_STR_1(name) __TO_STR_2(name)
#define TO_STR(name) __TO_STR_1(name)
#define FINISH_COMMANDS  static struct handler command_handler[__COUNTER__]

struct handler
{
    size_t hash;
    bool is_subcommands, allow_thread;
    union
    {
        bool (*handler)();
        struct {
            struct handler_subcommand {
                size_t hash;
                bool allow_thread;
                bool (*handler)();
            } *sub;
            size_t size;
        };
    };
};
