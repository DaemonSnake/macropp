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

#include "append.h"
#include "class.h"
#include "method.h"
#include "keywords.h"
#include "new.h"
#include "Type.h"

typedef struct Object *Object;
struct Object__private_data
{
    Type type;
};

struct Object__vtable {
    Type (*getType)(Object this);
    void (*dtor)(Object this);
};

struct Object__private {
    const struct Object__vtable * const _vtable;
    struct Object__private_data;
};

struct Object {
    const struct Object__vtable * const _vtable;
    char Object__private_data[sizeof(struct Object__private)];
};

Object Object__shallow_new();
void Object__ctor_Type(struct Object__private *this, Type type);

extern const struct Object__vtable Object__vtable_instance;
