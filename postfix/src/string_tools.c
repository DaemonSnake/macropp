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
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

int skip_separator(char *string)
{
    int size = (string ? strlen(string) : 0);

    if (size <= 0)
        return 0;
    for (int i = 0; i < size; i++)
        if (string[i] != ' ' && string[i] != '\t' && string[i] != '\n')
            return i;
    return 0;
}

char *min_str(char *l, char *r) { return ((l < r && l != NULL) || (r == NULL) ? l : r); }

char *get_argument(char *arg_list, int index)
{
    for (int i = 0; arg_list > (char *)0x4 && i < index; arg_list = (i++, strstr(arg_list, " @, ") + 4));
    if (arg_list <= (char *)0x4)
        return NULL;
    return strndup(arg_list, min_str(strstr(arg_list, " @, "), arg_list + strlen(arg_list)) - arg_list);
}

int string_index(char *motif, ...)
{
    va_list ap;
    char *tmp;

    if (motif == NULL)
        return -1;
    va_start(ap, motif);
    for (int index = 0; (tmp = va_arg(ap, char *)) != NULL; index++)
        if (strcmp(motif, tmp) == 0)
            return index;
    va_end(ap);
    return -1;
}

int int_index(char *str, char car)
{
    char *res = index(str, car);
    return (res == NULL ? -1 : res - str);
}

char *replace_special_characters(char *str)
{
    char *read = str;
    char *write = str;

    if (str == NULL)
        return NULL;
    for (write = str; *read != '\0'; read++, write++)
    {
        if (*read == '\\' && read[1] != '\0')
        {
            int i = int_index("abfnrtv'\"", read[1]);
            if (i == -1)
                goto no_backslash;
            *write = "\a\b\f\n\r\t\v'\""[i];
            read++;
        }
        else if (read != write)
        no_backslash:
            *write = *read;
    }
    *write = '\0';
    return str;
}

char *index_without_escape(char *str, char c)
{
    char *tmp = index(str, c);

    if (tmp == str)
        return tmp;
    while (tmp != NULL)
    {
        if (*(tmp - 1) != '\\')
            return tmp;
        tmp = index(tmp + 1, c);
    }
    return NULL;
}

char *append_string_n(char *or, char *new_end, int size_end)
{
    int size = (!or ? 0 : strlen(or));

    if (!or && !new_end)
        return NULL;
    if (or == NULL)
        return strndup(new_end, size_end);
    if (new_end == NULL)
        return or;
    or = realloc(or, size + size_end + 1);
    memmove(or + size, new_end, size_end);
    *(or + size_end + size) = '\0';
    return or;
}

char *append_string(char *or, char *new_end)
{
    return append_string_n(or, new_end, (!new_end ? 0 : strlen(new_end)));
}
