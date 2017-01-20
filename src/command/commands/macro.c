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
#include <pthread.h>

struct macro_node
{
    size_t hash;
    char *text;
    int size;
    struct macro_node *next;
    struct macro_node *prev;
};

static struct
{
    struct macro_node *front;
    struct macro_node *back;
} list = {NULL, NULL};

static void remove_macro(struct macro_node *it)
{
    if (!it)
        return ;
    if (it == list.front)
        list.front = it->next;
    else
    {
        it->prev->next = it->next;
        if (it->next)
            it->next->prev = it->prev;
    }
    FREE(it->text, it);
}

static struct macro_node *find_macro(size_t id)
{
    for (struct macro_node *it = list.front; it != NULL; it = it->next)
        if (it->hash == id)
            return it;
    return NULL;
}

static void create_macro(size_t id, char *text)
{
    struct macro_node *tmp = malloc(sizeof(struct macro_node));

    tmp->text = text;
    tmp->size = strlen(text);
    tmp->hash = id;
    tmp->next = list.front;
    list.front = tmp;
}

static char *eval_macro(char *value, bool eval)
{
    value = strdup(value);
    if (!eval)
        return value;
    return eval_string_command(value);
}

void expand_macro(buffer this, char *name)
{
    size_t hash = hash_string(name);
    struct macro_node *it = find_macro(hash);

    if (!it)
        return ;
    this->input_index += it->size;
    write(this->out, it->text, it->size);
}

static void update_macro(size_t hash, struct macro_node *it, char *value, bool eval)
{
    if (!it)
        create_macro(hash, eval_macro(value, eval));
    else
    {
        value = eval_macro(value, eval);
        free(it->text);
        it->text = value;
        it->size = strlen(value);
    }
}

NEW_SUBCOMMAND(MACRO_OP, UNDEF)
{
    char *name RAII = POP();
    struct macro_node *it = find_macro(hash_string(name));

    CLEAN();
    remove_macro(it);
    return true;
}

NEW_SUBCOMMAND(MACRO_OP, SET)
{
    size_t hash = POP(hash);
    char *opt RAII = POP();
    struct macro_node *it = find_macro(hash);

    CLEAN();
    update_macro(hash, it, opt, false);
    return true;
}

NEW_SUBCOMMAND(MACRO_OP, EVAL)
{
    size_t hash = POP(hash);
    char *opt RAII = POP();
    struct macro_node *it = find_macro(hash);

    CLEAN();
    update_macro(hash, it, opt, true);
    return true;
}

__attribute__((destructor))
static void clean_all_macros()
{
    struct macro_node *next = NULL;
    for (struct macro_node *it = list.front; it != NULL; it = next)
    {
        next = it->next;
        free(it->text);
        free(it);
    }
    list.front = NULL;
    list.back = NULL;
}
