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

#if __INCLUDE_LEVEL__ != 0
#pragma once
#endif

#include <stdlib.h>
#include <string.h>
#include "inc/Object.h"

class(String, Object)
{
    const char *c_str;
    size_t length;

    ctor(C)(const char *c_str)
    {
        $.c_str = strdup(c_str);
        $.length = strlen(c_str);
    }
    
    virtual const char * method(c_str)()
    {
        return $.c_str;
    }

    setter(c_str, value) {
        free((char *)$.c_str);
        $.c_str = strdup(value);
        $.length = strlen(value);
    }

    // TODO
    /* virtual c_str() char * { */
    /*     return $.c_str; */
    /* } */

    virtual char method(charAt)(int index)
    {
        if (index < 0 || index > (int)$.length)
            return '\0';
        else
            return $.c_str[index];
    }
}
