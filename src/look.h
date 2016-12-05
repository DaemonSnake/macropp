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

typedef enum {
    a_true = 1, b_true = 2, ab_false = 0
} trilean;

static bool ignore_literals(buffer this, trilean *in_literal, char *found);
static char *index_inf(char *str, char motif, char *other);
static void action(buffer this, action_type type, char **result, char *before);

#define ACTION(x...) action(this, type, &result, x)

#define NEW_READ                                \
    if ($.index >= $.size || $.size == 0)       \
    {                                           \
        ACTION(0);                              \
        __read(this);                           \
        continue;                               \
    }

#define END_OK                                  \
    return result;

#define END_KO                                          \
    {                                                   \
        if (result && type == COPY)                     \
        {                                               \
            print_strs(this, result, NULL);             \
            free(result);                               \
        }                                               \
        if ($.data)                                     \
        {                                               \
            print_strs(this, $.data + $.index, NULL);   \
            free($.data);                               \
            $.data = NULL;                              \
        }                                               \
        return 0;                                       \
    }
