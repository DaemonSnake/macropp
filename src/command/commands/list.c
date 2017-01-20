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
#include "tools/list.c"

NEW_SUBCOMMAND(LIST, PRINT)
{
    struct list_node *node = find_list_node(POP(hash));
    char *sep RAII = POP();

    CLEAN();
    print_list_id(this, node, (sep ? sep : " "));
    return true;
}

NEW_SUBCOMMAND(LIST, PUSH_FRONT)
{
    size_t hash = POP(hash);

    while (args.size > 0)
        list_push_back(hash, find_list_node(hash), POP());
    CLEAN();
    return true;
}

NEW_SUBCOMMAND(LIST, PUSH_BACK)
{
    size_t hash = POP(hash);

    while (args.size > 0)
        list_push_front(hash, find_list_node(hash), POP());
    CLEAN();
    return true;
}

NEW_SUBCOMMAND(LIST, POP_FRONT)
{
    struct list_node *node = find_list_node(POP(hash));

    CLEAN();
    list_pop_front(node);
    return true;
}

NEW_SUBCOMMAND(LIST, POP_BACK)
{
    struct list_node *node = find_list_node(POP(hash));

    CLEAN();
    list_pop_back(node);
    return true;
}

NEW_SUBCOMMAND(LIST, PARSE)
{
    size_t hash = POP(hash);
    char *value RAII = POP();

    CLEAN();
    list_parse_parenth(hash, find_list_node(hash), value);
    return true;
}

NEW_SUBCOMMAND(LIST, REMOVE)
{
    struct list_node *node = find_list_node(POP(hash));

    while (args.size > 0)
    {
        int value = POP(to_int);
        if (value < 0)
            break;
        list_remove_item(node, (unsigned)value);
    }
    CLEAN();
    return true;
}

NEW_SUBCOMMAND(LIST, ITEM)
{
    struct list_node *node = find_list_node(POP(hash));
    int value = POP(to_int);

    CLEAN();
    if (value < 0)
        return false;
    print_strs(this, list_get_item(node, (unsigned)value), NULL);
    return true;
}

NEW_SUBCOMMAND(LIST, CLEAR)
{
    struct list_node *node = find_list_node(POP(hash));

    CLEAN();
    list_clear(node);
    return true;
}

NEW_SUBCOMMAND(LIST, EVAL)
{
    struct list_node *node = find_list_node(POP(hash));
    int index = POP(to_int);

    CLEAN();
    if (index < 0)
        return false;
    list_eval(node, index);
    return true;
}
