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
#define NEW_SUBCOMMAND(name) bool CMANGLE(name)()

struct handler
{
    char *name;
    size_t hash;
    bool is_subcommands, allow_thread;
    union
    {
        bool (*handler)();
        struct {
            char *name;
            size_t hash;
            bool allow_thread;
            bool (*handler)();
        } sub[];
    };
};
