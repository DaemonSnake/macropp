/*
 * Copyright (C) 2016  Bastien Penavayre
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "macro++.h"
#include "header_command.h"

#define DEFAULT
#include "create_command.h"
{
    char *after RAII = POP(),
        *before RAII = POP();

    CLEAN();
    return (balanced_look_for(this, '{', '}', before, after, false, PROCCESS) != NULL);
}

static bool look_func(buffer this, struct array args, bool swallow)
{
    char *motif RAII = POP(),
        *after RAII = POP(),
        *before RAII = POP();

    CLEAN();
    return (look_for(this, motif, before, after, swallow, PROCCESS) != NULL);
}

#define COMMAND LOOK
#include "create_command.h"
{ return look_func(this, args, false); }

#define COMMAND L_SWALLOW
#include "create_command.h"
{ return look_func(this, args, true); }

static bool balenced_func(buffer this, struct array args, bool swallow)
{
    char *in_tmp RAII = POP(),
        *out_tmp RAII = POP(),
        *after RAII = POP(),
        *before RAII = POP(),
        in, out;

    CLEAN();
    unchar_string(in_tmp);
    unchar_string(out_tmp);
    if (in_tmp == NULL || out_tmp == NULL || after == NULL)
        return false;
    in = in_tmp[0];
    out = out_tmp[0];
    return (balanced_look_for(this, in, out, before, after, swallow, PROCCESS) != NULL);
}

#define COMMAND BALENCED
#include "create_command.h"
{ return balenced_func(this, args, false); }

#define COMMAND B_SWALLOW
#include "create_command.h"
{ return balenced_func(this, args, true); }

#define COMMAND COUNTER
#define SUBCOMMANDS                                       \
    subcommand(VALUE, true)                               \
    subcommand(NEXT, true)                                \
    subcommand(PREV, true)
#include "create_command.h"

#define COMMAND STRLEN
#define THREADS false
#include "create_command.h"
{
    char *str RAII = replace_special_characters(POP());
    char *begin, *end;

    CLEAN();
    if (str == NULL)
        return false;
    if ((begin = index(str, '"')) == NULL || (end = rindex(++begin, '"')) == NULL)
        return (print_size(this, 0), false);
    print_size(this, end - begin);
    return true;
}

#define COMMAND FORMAT
#include "create_command.h"
{
    char *format RAII = replace_special_characters(POP()),
        *end = rindex(format, '"'),
        *it = index(format, '"') + 1,
        *tmp;

    while(it > (char *)1)
    {
        tmp = index(it, '%');
        this->input_index += (tmp ? tmp: end) - it;
        write(this->out, it, (tmp ? tmp: end) - it);
        it = tmp;
        if (it && (tmp = POP()) != NULL) {
            print_strs(this, tmp, NULL);
            free(tmp);
        }
        it++;
    }
    CLEAN();
    return true;
}

#define COMMAND MACRO
#include "create_command.h"
{
    char *name RAII = POP();
    
    CLEAN();
    if (!is_identifier(name))
        return false;
    expand_macro(this, name);
    return true;
}

#define COMMAND MACRO_OP
#define SUBCOMMANDS                             \
    subcommand(UNDEF, false)                    \
    subcommand(SET, false)                      \
    subcommand(EVAL, false)
#include "create_command.h"

#define COMMAND LIST
#define SUBCOMMANDS                             \
    subcommand(PRINT, true)                     \
    subcommand(PUSH_FRONT, false)               \
    subcommand(PUSH_BACK, false)                \
    subcommand(POP_FRONT, false)                \
    subcommand(POP_BACK, false)                 \
    subcommand(PARSE, false)                    \
    subcommand(REMOVE, false)                   \
    subcommand(ITEM, true)                      \
    subcommand(CLEAR, false)                    \
    subcommand(EVAL, true)
#include "create_command.h"

#define COMMAND SYSTEM
#define THREADS false
#include "create_command.h"
{
    int fd[2];
    char tmp_buffer[2048];
    int len;

    if (pipe(fd) == -1) {
        CLEAN();
        return false;
    }
    switch (fork())
    {
    case -1:
        close(fd[0]);
        close(fd[1]);
        CLEAN();
        return false;
    case 0:
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execvp(args.data[1], &args.data[1]);
        CLEAN();
        exit(0);
    default:
        close(fd[1]);
        CLEAN();
        while ((len = read(fd[0], tmp_buffer, 2048)) > 0)
        {
            this->input_index += len;
            write(this->out, tmp_buffer, len);
        }
        close(fd[0]);
        break;
    }
    return true;
}

#define COMMAND SWITCH
#include "create_command.h"
{
    char *value_unparse RAII = POP(),
        *sep = index(value_unparse, ':');
    bool is_numb = (sep++ && strcmp(value_unparse, "number") == 0);
    double number;
    size_t hash = hash_string(value_unparse),
        default_hash = hash_string("@default");

    if (is_numb)
        number = atof(sep);
    while (args.size > 0)
    {
        char *value_unparse RAII = POP(),
            *value = index(value_unparse, ':');
        size_t it_hash = hash_string(value),
            diff = value - value_unparse;

        if (!value) continue;
        if ((is_numb && number_in_string(number, value_unparse, diff)) ||
            (!is_numb && (hash == it_hash || it_hash == default_hash))) {
            print_strs(this, value + 1, NULL);
            CLEAN();
            return true;
        }
    }
    CLEAN();
    return false;
}

FINISH_COMMANDS;

bool find_command(struct array *args, struct handler_content *ret)
{
    char *arg RAII = POP();
    size_t hash;

    if (!arg)
        return ERROR("command name required");
    hash = hash_string(arg);
    for (size_t i = 0; i < ARRAY_SIZE(command_handler); i++)
        if (hash == command_handler[i].hash)
        {
            AUTO(command, &command_handler[i]);
            if (command->is_subcommands)
            {
                char *subcommand RAII = POP();
                if (subcommand == NULL)
                    return ERROR("no subcommand name for '%s'", arg);
                hash = hash_string(subcommand);
                for (i = 0; i < command->size; i++)
                    if (command->sub[i].hash == hash)
                        return SUCCESS(command->sub[i]);
                return ERROR("subcommand '%s' not found", subcommand);
            }
            return SUCCESS(*command);
        }
    return ERROR("command '%s' not found", arg);
}

void handle_arguments(buffer this)
{
    struct array args = {NULL, NULL, 0};
    struct handler_content handler;

    if (!fill_argument_list(this, &args) || args.data == NULL)
        return ;
    if (!find_command(&args, &handler)) {
        CLEAN();
        return ;
    }
    if (handler.allow_thread)
        spawn_command(this, handler.handler, args);
    else
        handler.handler(this, args);
}
