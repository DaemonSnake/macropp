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

#define GET(i) get_argument(argument, i)
#define NEW_HANDLE(name) void handle_ ## name(buffer buf, char *argument)

NEW_HANDLE(default)
{
    char *after RAII = GET(0),
        *before RAII = GET(1);

    balanced_look_for(buf, '{', '}', before, false, PROCCESS);
    write(buf->out, after, strlen(after));
}

NEW_HANDLE(look)
{
    char *motif RAII = GET(0),
        *after RAII = GET(1),
        *before RAII = GET(2);
    
    look_for(buf, motif, before, false, PROCCESS);
    write(buf->out, after, strlen(after));
}

NEW_HANDLE(l_swallow)
{
    char *motif RAII = GET(0),
        *after RAII = GET(1),
        *before RAII = GET(2);
    
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
    char *to_mangle RAII = GET(1),
        *type_str RAII = GET(0);
    int type = string_index(type_str, "VALUE", "NEXT", "PREV", NULL);

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

NEW_HANDLE(unstring)
{
    char *str RAII = GET(0);
    char *begin, *end;

    if (str == NULL)
        return ;
    if ((begin = index(str, '"')) == NULL)
        return ;
    if ((end = rindex(begin + 1, '"')) == NULL)
        return ;
    if (begin + 1 == str)
        return ;
    write(buf->out, begin + 1, end - (begin + 1));
}

NEW_HANDLE(strlen)
{
    char *str RAII = replace_special_characters(GET(0));
    char *begin, *end;

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
}

#ifdef $
#undef $
#endif
#define $ handlers[i]

void handle_arguments(buffer buf, char *argument)
{
    struct {
        char *motif;
        int size;
        void (*handler)(buffer, char *);
        bool reads_input;
    } handlers[] = {
        {" ", 1, handle_default, true},
        {"LOOK ", 5, handle_look, true},
        {"LOOK_SW ", 8, handle_l_swallow, true},
        {"BALENCED ", 9, handle_balenced, true},
        {"BALENCED_SW ", 12, handle_b_swallow, true},
        {"COUNTER ", 8, handle_counter, false},
        {"UNSTRING ", 9, handle_unstring, false},
        {"STRLEN ", 6, handle_strlen, false},
        {"FORMAT ", 6, handle_format, false}
    };

    int size;
    if (argument == NULL)
        return ;

    size = strlen(argument);
    for (unsigned i = 0; i < (sizeof(handlers) / sizeof(handlers[0])); i++)
        if (size >= $.size && strncmp(argument, $.motif, $.size) == 0)
        {
            if ($.reads_input)
                spawn_command(buf, $.handler, argument, $.size);
            else
            {
                $.handler(buf, argument + $.size);
                free(argument);
            }
            break;
        }
}
