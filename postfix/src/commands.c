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
#include "inc.h"

#define GET(i) pop_argument(&args, i + 1)
#define CLEAN() free_arguments(&args)
#define NEW_HANDLE(name) void handle_ ## name(buffer buf, struct array args)

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
    balanced_look_for(buf, '{', '}', before, false, PROCCESS);
    write(buf->out, after, strlen(after));
}

NEW_HANDLE(look)
{
    char *motif RAII = GET(0),
        *after RAII = GET(1),
        *before RAII = GET(2);

    CLEAN();
    look_for(buf, motif, before, false, PROCCESS);
    write(buf->out, after, strlen(after));
}

NEW_HANDLE(l_swallow)
{
    char *motif RAII = GET(0),
        *after RAII = GET(1),
        *before RAII = GET(2);

    CLEAN();
    look_for(buf, motif, before, true, PROCCESS);
    write(buf->out, after, strlen(after));
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
        return ;
    in = in_tmp[0];
    out = out_tmp[0];
    balanced_look_for(buf, in, out, before, false, PROCCESS);
    write(buf->out, after, strlen(after));
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
        return ;
    in = in_tmp[0];
    out = out_tmp[0];
    balanced_look_for(buf, in, out, before, true, PROCCESS);
    write(buf->out, after, strlen(after));
}

NEW_HANDLE(counter)
{
    static size_t i = 0;
    char *type_str RAII = GET(0),
        *to_mangle RAII = GET(1);
    int type = string_index(type_str, "VALUE", "NEXT", "PREV", NULL);

    CLEAN();
    if (type < 0)
        return ;
    if (to_mangle)
        print_strs(buf->out, to_mangle, "__", NULL);
    if (type == 0)
        print_size(buf->out, i);
    else if (type == 1)
        print_size(buf->out, ++i);
    else
        print_size(buf->out, (i == 0 ? i : i - 1));
}

NEW_HANDLE(strlen)
{
    char *str RAII = replace_special_characters(GET(0));
    char *begin, *end;

    CLEAN();
    if (str == NULL)
        return ;
    if ((begin = index(str, '"')) == NULL || (end = rindex(++begin, '"')) == NULL)
        return (void)print_size(buf->out, 0);
    print_size(buf->out, end - begin);
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
        write(buf->out, it, (tmp ? tmp: end) - it);
        it = tmp;
        if (it && (tmp = GET(++i)) != NULL) {
            print_strs(buf->out, tmp, NULL);
            free(tmp);
        }
        it++;
    }
    CLEAN();
}

NEW_HANDLE(macro)
{
    char *name RAII = GET(0),
        *value RAII = GET(1);

    CLEAN();
    if (!is_identifier(name))
        return ;
    if (value)
        update_macro(name, value, false);
    else
        expand_macro(buf, name);
}

NEW_HANDLE(macro_eval)
{
    char *name RAII = GET(0),
        *value RAII = GET(1);

    CLEAN();
    if (!is_identifier(name))
        return ;
    (void)buf;
    update_macro(name, value, true);
}

static const struct handler_s handlers[] = {
    {"LOOK_SW", 7, handle_l_swallow, true},
    {"LOOK", 4, handle_look, true},
    {"BALENCED_SW", 11, handle_b_swallow, true},
    {"BALENCED", 8, handle_balenced, true},
    {"COUNTER", 7, handle_counter, false},
    {"STRLEN", 6, handle_strlen, false},
    {"FORMAT", 6, handle_format, true},
    {"MACRO_EVAL", 10, handle_macro_eval, false},
    {"MACRO", 5, handle_macro, false}
};

#ifdef $
#undef $
#endif
#define $ handlers[i]

void handle_arguments(buffer buf)
{
    struct array args = get_argument_list(buf);
    char *arg = pop_argument(&args, 0);
    int size;
    
    if (args.data == NULL)
        return ;
    size = strlen(arg);
    if (size == 0)
    {
        spawn_command(buf, handle_default, args);
        return ;
    }
    for (unsigned i = 0; i < (sizeof(handlers) / sizeof(handlers[0])); i++)
        if (size >= $.size && strncmp(arg, $.motif, $.size) == 0)
        {
            free(arg);
            if ($.allow_spawn)
                spawn_command(buf, $.handler, args);
            else
                $.handler(buf, args);
            return ;
        }
    free_arguments(&args);
}
