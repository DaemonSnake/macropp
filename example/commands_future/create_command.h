bool CMANGLE(command_handle)();
static struct handler command_handler[];

#ifndef SUBCOMMANDS
                            
__attribute__((constructor))
static void CMANGLE(constructor)()
{
    struct handler command_tool = {
        .hash = hash_string(TO_STR(COMMAND)),
        .is_subcommands = false,
# if (defined(THREADS) && THREADS == true)
        .allow_thread = true,
# else
        .allow_thread = false,
# endif
        .handler = CMANGLE(command_handle)
    };

    command_handler[__COUNTER__] = command_tool;
}

bool CMANGLE(command_handle)()

#else

# define subcommand(x, y) NEW_SUBCOMMAND(COMMAND, x);
    SUBCOMMANDS
# undef subcommand
# define subcommand(x, y) {hash_string(#x), y, CMANGLE(x ## __ ## subcommand)},

# define CREATE_COMMAND(counter)                                        \
    __attribute__((constructor))                                        \
    static void CMANGLE(constructor)()                                  \
    {                                                                   \
        struct handler_subcommand subs_tool[] = { SUBCOMMANDS };        \
        static struct handler_subcommand subs[ARRAY_SIZE(subs_tool)];   \
        struct assign_tool { struct handler_subcommand data[ARRAY_SIZE(subs_tool)]; }; \
        *((struct assign_tool *)subs) = *((struct assign_tool *)subs_tool); \
                                                                        \
        struct handler command_tool = {                                 \
            .hash = hash_string(TO_STR(COMMAND)),                       \
            .is_subcommands = true,                                     \
            .allow_thread = false,                                      \
            .sub = subs                                                 \
        };                                                              \
                                                                        \
        command_handler[counter] = command_tool;                        \
    }                                                                   \
                                                                        \
    bool CMANGLE(command_handle)()                                      \
    {                                                                   \
        size_t sub_hash;                                                \
                                                                        \
        for (size_t j = 0;                                              \
             j < ARRAY_SIZE(command_handler[counter].sub) - 1; j++)     \
            if (sub_hash == command_handler[counter].sub[j].hash)       \
            {                                                           \
                return true;                                            \
            }                                                           \
        return false;                                                   \
    }

CREATE_COMMAND(__COUNTER__)

#undef CREATE_COMMAND

#endif

#ifdef COMMAND
# undef COMMAND
#endif

#ifdef SUBCOMMANDS
# undef SUBCOMMANDS
#endif

#ifdef THREADS
# undef THREADS
#endif
