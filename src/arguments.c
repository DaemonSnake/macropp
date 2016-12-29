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

static int count_substr(char *haystack, char *needle)
{
    int count = 0;
    char *str_begin = NULL, *char_begin = NULL;
    char *tmp = NULL;

    str_begin = index(haystack, '"');
    char_begin = index(haystack, '\'');
    while ((tmp = strstr(haystack, needle)))
    {
        if ((!str_begin || tmp < str_begin) &&
            (!char_begin || tmp < char_begin))
        {
            count++;
            haystack = tmp + 1;
        }
        else
            haystack = min_str(str_begin, char_begin) + 1;

        if (str_begin && str_begin < haystack)
            str_begin = index(haystack, '"');
        if (char_begin && char_begin < haystack)
            char_begin = index(haystack, '\'');
    }
    return count;
}

static bool argument_list_is_ok(char *result)
{
    if (!result)
        return true;
    return (count_substr(result, IN_STR) + 1) == count_substr(result, OUT_STR);
}

static char *copy_argument_list(buffer this)
{
    char *result = look_for(this, OUT_STR, NULL, NULL, false, COPY);
    
    while (!argument_list_is_ok(result)) {
        char *tmp = look_for(this, OUT_STR, NULL, NULL, false, COPY);
        if (!tmp)
            break ;
        result = append_string(result, tmp);
    }
    return result;
}

static int min_str_3(char *_0, char *_1, char *_2)
{
    return (min_str(_0, _1) == _0 ?
            (min_str(_0, _2) == _0 ? 0 : 2) :
            (min_str(_1, _2) == _1 ? 1 : 2));
}

char *get_end_of_argument(char *list, bool rec, char **end, char *motifs[3])
{
    /* char *motifs[3] = {IN_STR, OUT_STR, SEP_STR}; */
    enum { IN = 0, OUT = 1, SEP = 2 };
    char *result[3];
    char *char_begin = NULL, *str_begin = NULL;

    while (list && *list)
    {
        result[IN] = strstr(list, motifs[IN]);
        result[OUT] = strstr(list, motifs[OUT]);
        result[SEP] = (!rec ? strstr(list, motifs[SEP]) : NULL);
        
        if (!result[IN] && !result[OUT] && !result[SEP])
            break ;
        
        char_begin = index(list, '\''); //without escape
        str_begin = index(list, '"'); //without escape
        int min = min_str_3(result[IN], result[OUT], result[SEP]);

        if ((char_begin && result[min] > char_begin) ||
            (str_begin && result[min] > str_begin))
        {
            bool is_char = char_begin && (!str_begin || (str_begin && char_begin < str_begin));
            char *tmp = index((is_char ? char_begin : str_begin) + 1,
                              (is_char ? '\'' : '"'));
            if (!tmp)
                break ;
            list = tmp + 1;
            continue ;
        }
        
        int size = strlen(motifs[min]);
        result[min] += size;
        if (min == IN)
        {
            list = get_end_of_argument(result[min], true, NULL, motifs);
            continue ;
        }
        if (end)
            *end = result[min] - size;
        return result[min];
    }
    if (end)
        *end = list + strlen(list);
    return (list + strlen(list));
}

static char *dup_argument(char *arg, size_t size)
{
    char *res = NULL;;
    const char *to_rep[] = {"%CLOSE", "%SEP", "%OPEN", "%NL"};
    const int to_rep_size[] = {6, 4, 5, 3};
    const char *with[] = {OUT_STR, SEP_STR, IN_STR, "\n"};
    const int with_size[] = {OUT_STR_SIZE, SEP_STR_SIZE, IN_STR_SIZE, 1};
    char *found[] = {NULL, NULL, NULL, NULL};
    unsigned min = 0;
    char tmp[size + 1];

    strncpy(tmp, arg, size);
    tmp[size] = '\0';
    arg = tmp;
    for (unsigned i = 0; i < sizeof(to_rep) / sizeof(to_rep[0]); i++)
        found[i] = strstr(arg, to_rep[i]);
    while (*arg)
    {
        min = 0;
        for (unsigned i = 1; i < sizeof(to_rep) / sizeof(to_rep[0]); i++)
            min = (min_str(found[min], found[i]) == found[i] ? i : min);
        if (found[min] == NULL)
            break ;
        res = append_string_n(res, arg, found[min] - arg);
        res = append_string_n(res, (char *)with[min], with_size[min]);
        arg = found[min] + to_rep_size[min];
        for (unsigned i = 0; i < sizeof(to_rep) / sizeof(to_rep[0]); i++)
            if (found[i] && found[i] < arg)
                found[i] = strstr(arg, to_rep[i]);
    }
    return (*arg ? append_string(res, arg) : res);
}

bool fill_argument_list_from_string(char *arg_list, struct array *res)
{
    char *tmp = NULL;
    char *end = NULL;
    unsigned i = 0, size = 8;
    char *motifs[3] = {IN_STR, OUT_STR, SEP_STR};

    if (!res || !arg_list)
        return false;
    free(res->data);
    res->size = 1;
    res->data = calloc(sizeof(char *), size);
    for (tmp = arg_list; *tmp != ' ' && *tmp != '\t' && *tmp != '\n' && *tmp != 0;
         tmp++);
    res->data[i++] = strndup(arg_list, tmp - arg_list);
    arg_list = tmp + 1;
    while ((tmp = skip_seperator(get_end_of_argument(arg_list, false, &end, motifs))))
    {
        end = skip_seperator_end(tmp, end - 1);
        if (i >= size)
            res->data = realloc(res->data, (size += 8) * sizeof(char *));
        res->data[i++] = dup_argument(arg_list, end - arg_list);
        if (!*tmp || !*end)
            break;
        arg_list = tmp;
    }
    res->data = realloc(res->data, (i + 1) * sizeof(char *));
    res->size = i;
    res->data[res->size] = NULL;
    return true;
}

bool fill_argument_list(buffer this, struct array *res)
{
    char *arg_list = copy_argument_list(this);
    bool result = fill_argument_list_from_string(arg_list, res);

    free(arg_list);
    return result;
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
