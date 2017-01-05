#include "inc.h"

#define COMMAND LIST
#define SUBCOMMANDS                              \
    subcommand(PUSH_FRONT, false)                \
    subcommand(PUSH_BACK, false)                 \
    subcommand(PRINT, true)
#include "create_command.h"

NEW_SUBCOMMAND(PUSH_FRONT)
{
}

#include "clean_command.h"

/*-----------------------------------*/

#define COMMAND SYSTEM
#define THREAD true

#include "create_command.h"
{
}
