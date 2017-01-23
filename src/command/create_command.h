static struct handler command_handler[];

#ifndef SUBCOMMANDS

# ifdef DEFAULT
#  define COMMAND DEFAULT
#  define COMMAND_STR ""
# else
#  define COMMAND_STR TO_STR(COMMAND)
# endif

bool CMANGLE(command_handle)(buffer, struct array);

# ifndef THREADS
#  define THREADS true
# elif THREADS != true
#  undef THREADS
#  define THREADS false
# endif

__attribute__((constructor))
static void CMANGLE(constructor)()
{
    struct handler command_tool = {
        .hash = hash_string(COMMAND_STR),
        .is_subcommands = false,
        .allow_thread = THREADS,
        .handler = CMANGLE(command_handle)
    };

    command_handler[__COUNTER__] = command_tool;
}

bool CMANGLE(command_handle)(buffer this __attribute__((unused)), struct array args)
# ifdef DISPATCH
#  define dispatch(NAME, ARGS...) { return NAME(this, args, ##ARGS); }
     DISPATCH
#  undef DISPATCH
# endif

#else

# define subcommand(x, y) NEW_SUBCOMMAND(COMMAND, x);
    SUBCOMMANDS
# undef subcommand
# define subcommand(x, y) {hash_string(#x), y, CMANGLE(x ## __ ## subcommand)},

__attribute__((constructor))
static void CMANGLE(constructor)()
{
    struct handler_subcommand subs_tool[] = { SUBCOMMANDS };
    static struct handler_subcommand subs[ARRAY_SIZE(subs_tool)];
    struct assign_tool { struct handler_subcommand data[ARRAY_SIZE(subs_tool)]; };
    *((struct assign_tool *)subs) = *((struct assign_tool *)subs_tool);

    struct handler command_tool = {
        .hash = hash_string(TO_STR(COMMAND)),
        .is_subcommands = true,
        .allow_thread = false,
        .sub = subs,
        .size = ARRAY_SIZE(subs_tool)
    };

    command_handler[__COUNTER__] = command_tool;
}

# undef subcommand

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

#ifdef DEFAULT
# undef DEFAULT
#endif

#ifdef COMMAND_STR
# undef COMMAND_STR
#endif
