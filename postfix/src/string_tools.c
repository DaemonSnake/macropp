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

static char *min_str(char *l, char *r) { return ((l < r && l != NULL) || (r == NULL) ? l : r); }

char *get_argument(char *arg_list, int index)
{
    for (int i = 0; arg_list > (char *)0x4 && i < index; arg_list = (i++, strstr(arg_list, " @, ") + 4));
    if (arg_list <= (char *)0x4)
        return NULL;
    return strndup(arg_list, min_str(strstr(arg_list, " @, "), arg_list + strlen(arg_list)) - arg_list);
}

void free_all(void *front, ...)
{
    va_list ap;

    free(front);
    va_start(ap, front);
    while ((front = va_arg(ap, void *)) != NULL)
        free(front);
    va_end(ap);
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
            int i = int_index("abfnrtv", read[1]);
            if (i == -1)
                goto no_backslash;
            *write = "\a\b\f\n\r\t\v"[i];
            read++;
        }
        else if (read != write)
        no_backslash:
            *write = *read;
    }
    *write = '\0';
    return str;
}

void print_strs(int fd, ...)
{
    va_list ap;
    char *str;

    va_start(ap, fd);
    while ((str = va_arg(ap, char *)) != NULL)
        write(fd, str, strlen(str));
    va_end(ap);
}

void print_size(int fd, size_t i)
{
    char *number;

    asprintf(&number, "%lu", i);
    write(fd, number, strlen(number));
    free(number);
}

void free_ptr(void *ptr)
{
    if (!ptr || !(*(void **)ptr))
        return ;
    free(*(void **)ptr);
    *(void **)ptr = NULL;
}
