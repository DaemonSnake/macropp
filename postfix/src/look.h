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
        if ($.size == 0)                                                \
            return ;                                                    \
        $.index = ($.index < 0 ? 0 :                                    \
                   ($.index > $.size ? $.size : $.index));              \
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

#define HANDLE_STR(IN_NAME, CH, FOUND)                                  \
    {                                                                   \
        char *str_index = index_without_escape($.data + $.index, CH);   \
                                                                        \
        if (IN_NAME)                                                    \
        {                                                               \
            if (str_index)                                              \
            {                                                           \
                $.index = str_index - $.data + 1;                       \
                IN_NAME = false;                                        \
                continue ;                                              \
            }                                                           \
            else                                                        \
            {                                                           \
                $.index = $.size;                                       \
                continue ;                                              \
            }                                                           \
        }                                                               \
        else if (str_index)                                             \
        {                                                               \
            char *begin_str = str_index;                                \
                                                                        \
            str_index = index_without_escape(begin_str + 1, CH);        \
            if (!FOUND)                                                 \
            {                                                           \
                if (str_index)                                          \
                {                                                       \
                    $.index = str_index - $.data + 1;                   \
                    continue;                                           \
                }                                                       \
                IN_NAME = true;                                         \
                $.index = $.size;                                       \
                continue ;                                              \
            }                                                           \
            else if (begin_str < FOUND)                                 \
            {                                                           \
                if (!str_index)                                         \
                {                                                       \
                    IN_NAME = true;                                     \
                    $.index = $.size;                                   \
                    continue;                                           \
                }                                                       \
                else if (FOUND < str_index)                             \
                {                                                       \
                    $.index = str_index - $.data + 1;                   \
                    continue ;                                          \
                }                                                       \
            }                                                           \
        }                                                               \
    }


#define NEW_READ                                \
    if ($.index >= $.size || $.size == 0)       \
    {                                           \
        action(0);                              \
        __read(this);                           \
        continue;                               \
    }

#define END_OK                                  \
    return copy;

#define END_KO                                          \
    {                                                   \
        if (copy && type == COPY)                       \
        {                                               \
            print_strs($.out, copy, NULL);              \
            free(copy);                                 \
        }                                               \
        if ($.data)                                     \
        {                                               \
            print_strs($.out, $.data + $.index, NULL);  \
            free($.data);                               \
            $.data = NULL;                              \
        }                                               \
        return 0;                                       \
    }
