#include "inc.h"
#include <stdio.h>

size_t hash_string(char *_str)
{
    unsigned char *str = (unsigned char *)_str;
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
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

#define BOOL_STR(value) (value ? "true" : "false")
#define AUTO(name, value) typeof(value) name = value
#define ERROR(str) fprintf(stderr, str), 42

int main(int ac, char **av)
{
    size_t hash = (ac >= 2 ? hash_string(av[1]) : 0L);

    if (hash == 0L)
        return ERROR("command name required\n");
    for (int i = 0; i < ARRAY_SIZE(command_handler); i++)
        if (hash == command_handler[i].hash)
        {
            AUTO(command, &command_handler[i]);
            if (command->is_subcommands)
            {
                if ((hash = (ac >= 3 ? hash_string(av[2]) : 0L)) == 0L)
                    return ERROR("no subcommand name\n");
                for (i = 0; i < command->size; i++)
                    if (command->sub[i].hash == hash)
                    {
                        printf("THREADS => %s\n", BOOL_STR(command->sub[i].allow_thread));
                        command->sub[i].handler();
                        return 0;
                    }
                return ERROR("subcommand name not found\n");
            }
            printf("THREADS => %s\n", BOOL_STR(command->allow_thread));
            command->handler();
            return 0;
        }
    return ERROR("command name not found\n");
}
