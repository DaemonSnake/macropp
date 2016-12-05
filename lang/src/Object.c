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
#include "../inc/Object.h"

#include <string.h>
#include <stdlib.h>

static Type Object__getType(struct Object__private *this) {
    return this->type;
}

static void Object__dtor(struct Object__private *this) {
    (void)this;
}

const struct Object__vtable Object__vtable_instance = {
    (Type(*)(Object))Object__getType,
    (void(*)(Object))Object__dtor
};

void Object__ctor_Type(struct Object__private *this, Type type)
{
    this->type = type;
}

Object Object__shallow_new()
{
    Object ret = malloc(sizeof(struct Object));
    bzero(ret, sizeof(struct Object));
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
    *(void **)&ret->_vtable = &Object__vtable_instance;
    #pragma GCC diagnostic pop
    return ret;
}
