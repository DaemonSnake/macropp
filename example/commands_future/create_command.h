bool CMANGLE(command_handle)();

#ifdef subcommand
# undef subcommand
#endif
#define subcommand(x, y) bool CMANGLE(x ## __ ## subcommand)();

#ifdef SUBCOMMANDS
    SUBCOMMANDS
#endif
#undef subcommand

static struct handler CMANGLE(handler)[] = {
    .name = COMMAND,
#ifdef SUBCOMMANDS
    .is_subcommands = true,
    .allow_thread = false,
# define subcommand(x, y) {#x, 0L, y, CMANGLE(x ## __ ## subcommand)},
    .sub = { SUBCOMMANDS {NULL, 0L, false, NULL} }
#else
    .is_subcommands = false,
# if (defined(THREADS) && THREADS == true)
    .allow_thread = true,
# else
    .allow_thread = false,
# endif
    .handler = CMANGLE(command_handle)
#endif
};

__attribute__((constructor))
static void CMANGLE(constructor)
{
    for (size_t i = 0; i < ARRAY_SIZE(CMANGLE(handler)); i++)
    {
        CMANGLE(handler)[i].hash = hash_string(CMANGLE(handler)[i].name);
        #ifdef SUBCOMMANDS
        for (size_t j = 0; j < ARRAY_SIZE(CMANGLE(handler)[i].sub) - 1; j++)
            CMANGLE(handler)[i].sub[j].hash = hash_string(CMANGLE(handler)[i].sub[j].name);
        #endif
    }
}

#ifndef SUBCOMMANDS
bool CMANGLE(command_handle)()
#else
bool CMANGLE(command_handle)(struct array args)
{
    size_t hash, sub_hash;
    
    for (size_t i = 0; i < ARRAY_SIZE(CMANGLE(handler)); i++)
        if (hash == CMANGLE(handler)[i].hash)
            for (size_t j = 0; j < ARRAY_SIZE(CMANGLE(handler)[i].sub) - 1; j++)
                if (sub_hash == CMANGLE(handler)[i].sub[j].hash)
                {
                    return true;
                }
    return false;
}
#endif

