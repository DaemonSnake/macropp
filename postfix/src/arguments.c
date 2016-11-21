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

static char *copy_argument_list(buffer this)
{
    char *result = look_for(this, " @]", NULL, true, COPY);
    char *found = result;
    int count = 0;

    if (result == NULL)
        return NULL;
    while ((found = strstr(found, "[@")) != NULL) {
        count++;
        found += 2;
    }
    while (count > 0)
    {
        found = look_for(this, " @]", NULL, true, COPY);
        if (!found)
            return result;
        result = append_string_n(result, " @]", 3);
        result = append_string(result, found);
        count--;
    }
    return result;
}

static int min_str_3(char *_0, char *_1, char *_2)
{
    return (min_str(_0, _1) == _0 ?
            (min_str(_0, _2) == _0 ? 0 : 2) :
            (min_str(_1, _2) == _1 ? 1 : 2));
}

static char *get_end_of_argument(char *list, bool rec, char **end)
{
    char *motifs[3] = {"[@", " @]", " @, "};
    enum { IN = 0, OUT = 1, SEP = 2 };
    char *result[3];

    while (list && *list &&
           ((intptr_t)(result[IN] = strstr(list, motifs[IN])) +
            (intptr_t)(result[OUT] = strstr(list, motifs[OUT])) +
            (intptr_t)(result[SEP] = (!rec ? strstr(list, motifs[SEP]) : NULL))) != 0)
    {
        int min = min_str_3(result[IN], result[OUT], result[SEP]);
        if (end)
            *end = result[min];
        result[min] += strlen(motifs[min]);
        if (min == IN)
        {
            list = get_end_of_argument(result[min], true, NULL);
            continue ;
        }
        return result[min];
    }
    if (end)
        *end = list + strlen(list);
    return (list + strlen(list));
}

struct array get_argument_list(buffer this)
{
    char *arg_list = copy_argument_list(this),
        *tmp = NULL, *begin = arg_list;
    char **result = NULL, *end = NULL;
    unsigned i = 0;

    if (!arg_list || !(tmp = index(arg_list, ' ')))
        goto end;
    result = calloc(sizeof(char *), 1);
    result[i++] = strndup(arg_list, tmp - arg_list);
    arg_list = tmp + 1;
    while ((tmp = get_end_of_argument(arg_list, false, &end)))
    {
        result = realloc(result, (i + 1) * sizeof(char *));
        result[i++] = strndup(arg_list, end - arg_list);
        if (!*tmp || !*end)
            break;
        arg_list = tmp;
    }
 end:
    free(begin);
    return (struct array){result, i};
}

void free_arguments(struct array *arg)
{
    for (unsigned i = 0; i < arg->size; i++) {
        if (!arg->data[i])
            continue;
        free(arg->data[i]);
        arg->data[i] = NULL;
    }
    free(arg->data);
    arg->data = NULL;
    arg->size = 0;
}

char *pop_argument(struct array *arg, unsigned index)
{
    if (!arg || index >= arg->size)
        return NULL;
    char *tmp = arg->data[index];
    arg->data[index] = NULL;
    return tmp;
}
