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

#define GET(i) pop_argument(&args, i + 1)
#define ARGS_SIZE (args.size - 1)
#define CLEAN() free_arguments(&args)
#define NEW_HANDLE(name) bool handle_ ## name(buffer buf, struct array args)

struct handler_s {
    char *motif;
    int size;
    bool (*handler)(buffer, struct array);
    bool allow_spawn;
};

static const struct handler_s handlers[];

NEW_HANDLE(default)
{
    char *after RAII = GET(0),
        *before RAII = GET(1);

    CLEAN();
    return (balanced_look_for(buf, '{', '}', before, after, false, PROCCESS) != NULL);
}

NEW_HANDLE(look)
{
    char *motif RAII = GET(0),
        *after RAII = GET(1),
        *before RAII = GET(2);

    CLEAN();
    return (look_for(buf, motif, before, after, false, PROCCESS) != NULL);
}

NEW_HANDLE(l_swallow)
{
    char *motif RAII = GET(0),
        *after RAII = GET(1),
        *before RAII = GET(2);

    CLEAN();
    return (look_for(buf, motif, before, after, true, PROCCESS) != NULL);
}

NEW_HANDLE(balenced)
{
    char *in_tmp RAII = GET(0),
        *out_tmp RAII = GET(1),
        *after RAII = GET(2),
        *before RAII = GET(3),
        in, out;

    CLEAN();
    unchar_string(in_tmp);
    unchar_string(out_tmp);
    if (in_tmp == NULL || out_tmp == NULL || after == NULL)
        return false;
    in = in_tmp[0];
    out = out_tmp[0];
    return (balanced_look_for(buf, in, out, before, after, false, PROCCESS) != NULL);
}

NEW_HANDLE(b_swallow)
{
    char *in_tmp RAII = GET(0),
        *out_tmp RAII = GET(1),
        *after RAII = GET(2),
        *before RAII = GET(3),
        in, out;

    CLEAN();
    unchar_string(in_tmp);
    unchar_string(out_tmp);
    if (in_tmp == NULL || out_tmp == NULL || after == NULL)
        return false;
    in = in_tmp[0];
    out = out_tmp[0];
    return (balanced_look_for(buf, in, out, before, after, true, PROCCESS) != NULL);
}

NEW_HANDLE(counter)
{
    static size_t i = 0;
    char *type_str RAII = GET(0),
        *to_mangle RAII = GET(1);
    int type = string_index(type_str, "VALUE", "NEXT", "PREV", NULL);

    CLEAN();
    if (type < 0)
        return false;
    if (to_mangle)
        print_strs(buf, to_mangle, "__", NULL);
    if (type == 0)
        print_size(buf, i);
    else if (type == 1)
        print_size(buf, ++i);
    else
        print_size(buf, (i == 0 ? i : i - 1));
    return true;
}

NEW_HANDLE(strlen)
{
    char *str RAII = replace_special_characters(GET(0));
    char *begin, *end;

    CLEAN();
    if (str == NULL)
        return false;
    if ((begin = index(str, '"')) == NULL || (end = rindex(++begin, '"')) == NULL)
        return (print_size(buf, 0), false);
    print_size(buf, end - begin);
    return true;
}

NEW_HANDLE(format)
{
    char *format RAII = replace_special_characters(GET(0)),
        *end = rindex(format, '"'),
        *it = index(format, '"') + 1,
        *tmp;
    int i = 0;

    while(it > (char *)1)
    {
        tmp = index(it, '%');
        buf->input_index += (tmp ? tmp: end) - it;
        write(buf->out, it, (tmp ? tmp: end) - it);
        it = tmp;
        if (it && (tmp = GET(++i)) != NULL) {
            print_strs(buf, tmp, NULL);
            free(tmp);
        }
        it++;
    }
    CLEAN();
    return true;
}

NEW_HANDLE(macro)
{
    char *name RAII = GET(0);
    
    CLEAN();
    if (!is_identifier(name))
        return false;
    expand_macro(buf, name);
    return true;
}

NEW_HANDLE(macro_op)
{
    (void)buf;
    macro_handling(args);
    return true;
}

NEW_HANDLE(list)
{
    char *subc RAII = GET(0),
        *name RAII = GET(1);
    size_t hash = hash_string(name);
    bool ret = true;

    if (strcmp(subc, "PRINT") == 0) {
        char *sep RAII = GET(2);
        print_list_id(buf, hash, (sep ? sep : " "));
    }
    else if (strcmp(subc, "PUSH_BACK") == 0)
        list_push_back(hash, GET(2));
    else if (strcmp(subc, "PUSH_FRONT") == 0)
        list_push_front(hash, GET(2));
    else if (strcmp(subc, "POP_BACK") == 0)
        list_pop_back(hash);
    else if (strcmp(subc, "POP_FRONT") == 0)
        list_pop_front(hash);
    else if (strcmp(subc, "PARSE") == 0) {
        char *value RAII = GET(2);
        list_parse_parenth(hash, value);
    }
    else if (strcmp(subc, "REMOVE") == 0)
    {
        for (unsigned i = 2; i < ARGS_SIZE; i++)
        {
            char *index_str RAII = GET(i);
            int value = atoi(index_str);
            if (value < 0)
                break;
            list_remove_item(hash, (unsigned)value);
        }
    }
    else if (strcmp(subc, "ITEM") == 0)
    {
        char *index_str RAII = GET(2);
        int value = atoi(index_str);
        if (value >= 0)
            print_strs(buf, list_get_item(hash, (unsigned)value), NULL);
        else
            ret = false;
    }
    else if (strcmp(subc, "CLEAR") == 0)
        list_clear(hash);
    else
        ret = false;
    CLEAN();
    return ret;
}

NEW_HANDLE(system)
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
            buf->input_index += len;
            write(buf->out, tmp_buffer, len);
        }
        close(fd[0]);
        break;
    }
    return true;
}

NEW_HANDLE(switch)
{
    char *value_unparse RAII = GET(0),
        *sep = index(value_unparse, ':');
    bool is_numb = (sep && strncmp(value_unparse, "number", sep++ - value_unparse) == 0);
    double number;
    char *text = value_unparse;

    if (is_numb)
        number = atof(sep);
    for (size_t i = 1; i < ARGS_SIZE; i++)
    {
        char *value_unparse RAII = GET(i),
            *value = index(value_unparse, ':');
        size_t diff = value - value_unparse;

        if (!value) continue;
        if ((is_numb && number_in_string(number, value_unparse, diff)) ||
            (!is_numb && strncmp(value_unparse, text, diff) == 0) ||
            (diff == 7 && strncmp(value_unparse, "@default", diff) == 0)) {
            print_strs(buf, value + 1, NULL);
            CLEAN();
            return true;
        }
    }
    CLEAN();
    return false;
}

static const struct handler_s handlers[] = {
    {"LOOK_SW", 7, handle_l_swallow, true},
    {"LOOK", 4, handle_look, true},
    {"BALENCED_SW", 11, handle_b_swallow, true},
    {"BALENCED", 8, handle_balenced, true},
    {"COUNTER", 7, handle_counter, false},
    {"STRLEN", 6, handle_strlen, false},
    {"FORMAT", 6, handle_format, true},
    {"MACRO_OP", 8, handle_macro_op, false},
    {"MACRO", 5, handle_macro, false},
    {"LIST", 4, handle_list, false},
    {"SYSTEM", 6, handle_system, false},
    {"SWITCH", 6, handle_switch, true}
};

static const struct handler_s default_handler = { NULL, 0, handle_default, true };

#ifdef $
#undef $
#endif
#define $ handlers[i]

const struct handler_s *find_handler(char *arg)
{
    int size = (arg ? strlen(arg) : 0);

    if (size == 0)
        return &default_handler;
    for (unsigned i = 0; i < (sizeof(handlers) / sizeof(handlers[0])); i++)
        if (size >= $.size && strncmp(arg, $.motif, $.size) == 0)
            return &$;
    return NULL;
}

void handle_arguments(buffer buf)
{
    struct array args = {NULL, 0};
    char *arg;
    const struct handler_s * handler;

    if (!fill_argument_list(buf, &args) || args.data == NULL)
        return ;
    arg = pop_argument(&args, 0);
    handler = find_handler(arg);
    free(arg);
    if (!handler) {
        free_arguments(&args);
        return ;
    }
    if (handler->allow_spawn)
        spawn_command(buf, handler->handler, args);
    else
        handler->handler(buf, args);
}
