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
#pragma once

#define ACTION                                                          \
    void action(char *before)                                           \
    {                                                                   \
        if (type == COPY) {                                             \
            char *tmp = 0;                                              \
            asprintf(&tmp, "%s%.*s%s", (copy == 0 ? "" : copy),         \
                     $.index, $.data, (before ? before : ""));          \
            free(copy);                                                 \
            copy = tmp;                                                 \
            discard(this);                                              \
        }                                                               \
        else if (type == PROCCESS) {                                    \
            proccess(this);                                             \
            if (before)                                                 \
                write($.out, before, strlen(before));                   \
            copy = (void*)0x42;                                         \
        }                                                               \
    }

#define HANDLE_STR                                                      \
    {                                                                   \
        char *str_index = index($.data + $.index, '"');                 \
                                                                        \
        if (!in_str && str_index)                                       \
        {                                                               \
            skip_index = str_index + 1;                                 \
            in_str = true;                                              \
        }                                                               \
        else if (in_str)                                                \
        {                                                               \
            if (str_index == NULL)                                      \
            {                                                           \
                $.index = $.size;                                       \
                action(0);                                               \
                continue;                                               \
            }                                                           \
            else                                                        \
            {                                                           \
                $.index = str_index - $.data + 1;                       \
                skip_index = NULL;                                      \
                action(0);                                               \
                in_str = false;                                         \
                continue;                                               \
            }                                                           \
        }                                                               \
        else                                                            \
            skip_index = NULL;                                          \
    }

#define END_OK                                  \
    return copy;

#define END_KO                                  \
    {                                           \
        if (copy && type == COPY)               \
        {                                       \
            write($.out, copy, strlen(copy));   \
            free(copy);                         \
        }                                       \
        return 0;                               \
    }
