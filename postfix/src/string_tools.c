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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

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

void raii_free(void *arg)
{
    free(*(void **)arg);
    *(void **)arg = 0;
}
