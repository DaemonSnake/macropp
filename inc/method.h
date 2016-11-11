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

/* VIRTUAL */

#define ___METHOD_ADD_THIS(args...)                     \
    (CLASS() this, ##args),                             \
        (struct CLASS(__private) *this, ##args),

#define method(name)                            \
    , name, ___METHOD_ADD_THIS

PROC(define ___CALL_VIRTUAL(line, ret, name, arg_list, arg_list_p, body...) \
     PROC_2(define MACRO_GLUE_2_(METHODS_VTABLE_TOOL__, line)           \
            ret (*name) arg_list) _PRAGMA(_)                            \
     PROC_2(define MACRO_GLUE_2_(METHODS_NAME_TOOL__, line)             \
            . name = (ret (*) arg_list)&CLASS(__ ## name)) _PRAGMA(_) \
     PROC_2(define MACRO_GLUE_2_(METHODS_CODE_TOOL__, line)             \
            ret CLASS(__ ## name) arg_list_p body) _PRAGMA(_))

#define virtual                                                         \
    append_macro_dotcoma(METHODS_VTABLE, MACRO_GLUE(METHODS_VTABLE_TOOL__, __LINE__)) \
    append_macro(METHODS_NAME, MACRO_GLUE(METHODS_NAME_TOOL__, __LINE__)) \
    append_macro_nosep(METHODS_CODE, MACRO_GLUE(METHODS_CODE_TOOL__, __LINE__)) \
    __POSTFIX__(@ ) @) ___CALL_VIRTUAL ( __LINE__,

/* GET/SET */

#define getter(name)                                            \
    virtual typeof(((struct CLASS(__private) *)0)->name) method(get_ ## name)()

#define setter(name, value)                                             \
    virtual void method(set_ ## name)(typeof(((struct CLASS(__private) *)0)->name) value)

//TODO: axM

/* OVERRIDE */

/* PROC(define ___CALL_OVERRIDE(line, ret, name, arg_list, arg_list_p, body...) \ */
/*      PROC_2(define MACRO_GLUE_2_(METHODS_NAME_TOOL__, line)             \ */
/*             . name = (ret (*) arg_list)&CLASS(__ ## name)) _PRAGMA(_)   \ */
/*      PROC_2(define MACRO_GLUE_2_(METHODS_CODE_TOOL__, line)             \ */
/*             ret CLASS(__ ## name) arg_list_p body) _PRAGMA(_)) */

/* #define override                                                         \ */
/*     append_macro(METHODS_NAME, MACRO_GLUE(METHODS_NAME_TOOL__, __LINE__)) \ */
/*     append_macro_nosep(METHODS_CODE, MACRO_GLUE(METHODS_CODE_TOOL__, __LINE__)) \ */
/*     __POSTFIX__(@ ) @) ___CALL_OVERRIDE ( __LINE__, */

/* '*TOR' */

#define ctor(name)                                                      \
    append_macro_nosep(METHODS_CODE, MACRO_GLUE(METHODS_CODE_TOOL__, __LINE__)) \
    append_macro_nosep(METHODS_PROTO_CODE, MACRO_GLUE(METHODS_CODE_PROTO_TOOL__, __LINE__)) \
    __POSTFIX__(@ ) @) ___CALL_TOR ( __LINE__, ctor_ ## name, ___METHOD_ADD_THIS

PROC(define ___CALL_TOR(line, name, arg_list, arg_list_p, body...) \
     PROC_2(define MACRO_GLUE_2_(METHODS_CODE_TOOL__, line)             \
            void CLASS(__ ## name) arg_list_p body) _PRAGMA(_)           \
     PROC_2(define MACRO_GLUE_2_(METHODS_CODE_PROTO_TOOL__, line)       \
            void CLASS(__ ## name) arg_list_p;) _PRAGMA(_))

//TODO: DTOR
