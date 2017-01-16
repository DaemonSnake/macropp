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
#define ERROR(str) fprintf(stderr, "[ERROR from %s:%d] %s", __func__, __LINE__, str), false
#define SUCCESS(content) (*ret = (struct handler_content){(content).allow_thread, (content).handler}), true

struct handler_content
{
    bool allow_thread;
    bool (*handler)(buffer, struct array);
};

bool find_command(struct array args, struct handler_content *ret)
{
    size_t hash = (args.size >= 1 ? hash_string(args.data[0]) : 0L);

    if (hash == 0L)
        return ERROR("command name required\n");
    for (int i = 0; i < ARRAY_SIZE(command_handler); i++)
        if (hash == command_handler[i].hash)
        {
            AUTO(command, &command_handler[i]);
            if (command->is_subcommands)
            {
                if ((hash = (args.size >= 2 ? hash_string(args.data[1]) : 0L)) == 0L)
                    return ERROR("no subcommand name\n");
                for (i = 0; i < command->size; i++)
                    if (command->sub[i].hash == hash)
                        return SUCCESS(command->sub[i]);
                return ERROR("subcommand name not found\n");
            }
            return SUCCESS(*command);
        }
    return ERROR("command name not found\n");
}

int main(int ac, char **av)
{
    struct handler_content handler;

    find_command((struct array){av + 1, ac - 1}, &handler);
}
