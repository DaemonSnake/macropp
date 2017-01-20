/*
 * Copyright (C) 2017  Bastien Penavayre
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

#define __TO_STR_2(name) #name
#define __TO_STR_1(name) __TO_STR_2(name)
#define TO_STR(name) __TO_STR_1(name)
#define FINISH_COMMANDS  static struct handler command_handler[__COUNTER__]
#define ERROR(error, name...)                                           \
    fprintf(stderr, "[ERROR %s:%d] ", __func__, __LINE__),              \
        fprintf(stderr, error, ##name), false
#define AUTO(name, value) typeof(value) name = value
#define SUCCESS(value)                                                  \
    (*ret = (struct handler_content){(value).allow_thread, (value).handler}), true

struct handler
{
    size_t hash;
    bool is_subcommands, allow_thread;
    union
    {
        bool (*handler)(buffer, struct array);
        struct {
            struct handler_subcommand {
                size_t hash;
                bool allow_thread;
                bool (*handler)(buffer, struct array);
            } *sub;
            size_t size;
        };
    };
};

struct handler_content
{
    bool allow_thread;
    bool (*handler)(buffer, struct array);
};
