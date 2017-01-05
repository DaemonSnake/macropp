#include "inc.h"

size_t hash_string(char *name)
{
    return 0L;
}

#define COMMAND LIST
#define SUBCOMMANDS                              \
    subcommand(PUSH_FRONT, false)                \
    subcommand(PUSH_BACK, false)                 \
    subcommand(PRINT, true)
#include "create_command.h"

NEW_SUBCOMMAND(LIST, PUSH_FRONT)
{
}

NEW_SUBCOMMAND(LIST, PUSH_BACK)
{
}

NEW_SUBCOMMAND(LIST, PRINT)
{
}

/*-----------------------------------*/

#define COMMAND SYSTEM
#define THREADS true

#include "create_command.h"
{
}

FINISH_COMMANDS;

int main()
{
}
