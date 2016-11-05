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

#include <string.h>

#define class(name, super)                                      \
    typedef struct name *name;                                  \
    append_macro(OBJECTS, name)                                 \
    UNDEF(CLASS)                                                \
    PROC(define CLASS(x) MACRO_GLUE_2_(name, x))                \
                                                                \
    struct name ## __private_data __POSTFIX__(@ end_class @)

#define end_class                                                       \
    ;                                                                   \
                                                                        \
    struct CLASS(__vtable) {                                            \
        _PRAGMA(_) PROC_2(ifdef METHODS_VTABLE)                         \
        _PRAGMA(_) METHODS_VTABLE;                                      \
        _PRAGMA(_) PROC_2(endif) _PRAGMA(_)                             \
    };                                                                  \
                                                                        \
    struct CLASS(__private) {                                           \
        const struct CLASS(__vtable) * const _vtable;                   \
        struct CLASS(__private_data);                                   \
    };                                                                  \
                                                                        \
    struct CLASS() {                                                    \
        const struct CLASS(__vtable) * const _vtable;                   \
        char CLASS(__private_data)[sizeof(struct CLASS(__private))];    \
    };                                                                  \
                                                                        \
    _PRAGMA(_) PROC(if __INCLUDE_LEVEL__ == 0)                          \
                                                                        \
    _PRAGMA(_) PROC_2(ifdef METHODS_CODE)                               \
    _PRAGMA(_) METHODS_CODE;                                            \
    _PRAGMA(_) PROC_2(endif) _PRAGMA(_)                                 \
                                                                        \
    const struct CLASS(__vtable) CLASS(__vtable_instance) = {           \
        _PRAGMA(_) PROC_2(ifdef METHODS_NAME)                           \
        _PRAGMA(_) METHODS_NAME                                         \
        _PRAGMA(_) PROC_2(endif) _PRAGMA(_)                             \
    };                                                                  \
                                                                        \
    CLASS() CLASS(__shallow_new)()                                      \
    {                                                                   \
        CLASS() ret = malloc(sizeof(struct CLASS()));                   \
        bzero(ret, sizeof(struct CLASS()));                             \
        _Pragma("GCC diagnostic push");                                 \
        _Pragma("GCC diagnostic ignored \"-Wdiscarded-qualifiers\"");   \
        *(void **)&ret->_vtable = &CLASS(__vtable_instance);            \
        _Pragma("GCC diagnostic pop");                                  \
        return ret;                                                     \
    }                                                                   \
                                                                        \
    _PRAGMA(_) PROC(else)                                               \
                                                                        \
    CLASS() CLASS(__shallow_new)();                                     \
    METHODS_PROTO_CODE                                                  \
                                                                        \
    _PRAGMA(_) PROC(endif)                                              \
                                                                        \
         _PRAGMA(_) PROC_2(undef METHODS_CODE)                          \
         _PRAGMA(_) PROC_2(undef METHODS_NAME)                          \
         _PRAGMA(_) PROC_2(undef METHODS_VTABLE)                        \
         _PRAGMA(_) PROC_2(undef METHODS_PROTO_CODE) _PRAGMA(_)
