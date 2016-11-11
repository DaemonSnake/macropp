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

void handle_default(buffer buf, char *argument)
{
    char *after = get_argument(argument, 0),
        *before = get_argument(argument, 1);

    spawn_balenced_look(buf, '{', '}', before, after);
}

void handle_look(buffer buf, char *argument)
{
    (void)buf, (void)argument;
}

void handle_balenced(buffer buf, char *argument)
{
    (void)buf, (void)argument;
}

void handle_arguments(buffer buf, char *argument)
{
    char *motif[] = {" ", "LOOK ", "BALENCED "};
    int sizes[] = {1, 5, 9}, size;
    void (*handlers[])(buffer, char *) =
        {handle_default, handle_look, handle_balenced};

    if (argument == NULL)
        return ;

    size = strlen(argument);
    for (int i = 0; i < 3; i++)
        if (size >= sizes[i] && strncmp(argument, motif[i], sizes[i]) == 0)
        {
            handlers[i](buf, argument + sizes[i]);
            break;
        }
    free(argument);
}
