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

/* METHOD */

PROC(define ___METHOD_ADD_THIS(args...)         \
     (CLASS() this, ##args),                    \
     (struct CLASS(__private) *this, ##args))

PROC(define ___CALL_METHOD(line, name, arg_list, arg_list_p, ret, body...) \
     PROC_2(define MACRO_GLUE_2_(METHODS_VTABLE_TOOL__, line)           \
            ret (*name) arg_list) _PRAGMA(_)                            \
     PROC_2(define MACRO_GLUE_2_(METHODS_NAME_TOOL__, line)             \
            . name = (ret (*) arg_list)&CLASS(__ ## name)) _PRAGMA(_) \
     PROC_2(define MACRO_GLUE_2_(METHODS_CODE_TOOL__, line)             \
            ret CLASS(__ ## name) arg_list_p body) _PRAGMA(_))

PROC(define ___CALL_METHOD2(line, name, arg_list, ret, body...)        \
     ___CALL_METHOD(line, name, arg_list, ret, body))

#define method                                                         \
    append_macro_dotcoma(METHODS_VTABLE, MACRO_GLUE(METHODS_VTABLE_TOOL__, __LINE__)) \
    append_macro(METHODS_NAME, MACRO_GLUE(METHODS_NAME_TOOL__, __LINE__)) \
    append_macro_nosep(METHODS_CODE, MACRO_GLUE(METHODS_CODE_TOOL__, __LINE__)) \
    ___CALL_METHOD2 (                                                   \
                     [@ ) @, , @]                            \
    [@BALENCED ( @, ) @, , @, ,___METHOD_ADD_THIS @]         \
    __LINE__,

/* ABSTRACT */

PROC(define ___CALL_ABSTRACT(line, name, arg_list, arg_list_p, ret)     \
     PROC_2(define MACRO_GLUE_2_(METHODS_VTABLE_TOOL__, line)           \
            ret (*name) arg_list) _PRAGMA(_)                            \
     PROC_2(define MACRO_GLUE_2_(METHODS_NAME_TOOL__, line)             \
            . name = (ret (*) arg_list)0 _PRAGMA(_)))

PROC(define ___CALL_ABSTRACT2(line, name, arg_list, ret)        \
     ___CALL_ABSTRACT(line, name, arg_list, ret))

#define abstract                                                        \
    append_macro_dotcoma(METHODS_VTABLE, MACRO_GLUE(METHODS_VTABLE_TOOL__, __LINE__)) \
    append_macro(METHODS_NAME, MACRO_GLUE(METHODS_NAME_TOOL__, __LINE__)) \
    ___CALL_ABSTRACT2 (                                                 \
                       [@LOOK_SW ; @, ) @]                              \
    [@BALENCED ( @, ) @, , @, ,___METHOD_ADD_THIS @]                    \
    __LINE__,


/* GET/SET */

#define getter(name)                                                    \
    method get_ ## name() typeof(((struct CLASS(__private) *)0)->name)

#define setter(name, value)                                             \
    method set_ ## name(typeof(((struct CLASS(__private) *)0)->name) value) void

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
/*     [@ ) @] ___CALL_OVERRIDE ( __LINE__, */

/* '*TOR' */

PROC(define __IN_CTOR__                                                 \
     {                                                                  \
         if (!this) this = (struct CLASS(__private) * )CLASS(__shallow_new)(); \
         void __ctor_tool__())

PROC(define __OUT_CTOR__ \
     ; __ctor_tool__();  \
     return (CLASS())this; \
     })

#define ctor(name)                                                      \
    append_macro_nosep(METHODS_CODE, MACRO_GLUE(METHODS_CODE_TOOL__, __LINE__)) \
    append_macro_nosep(METHODS_PROTO_CODE, MACRO_GLUE(METHODS_CODE_PROTO_TOOL__, __LINE__)) \
    ___CALL_TOR2 (                                                      \
                  [@ __OUT_CTOR__) @, ,__IN_CTOR__ @]                   \
    __LINE__, ctor_ ## name, ___METHOD_ADD_THIS

PROC(define ___CALL_TOR(line, name, arg_list, arg_list_p, body...) \
     PROC_2(define MACRO_GLUE_2_(METHODS_CODE_TOOL__, line)             \
            CLASS() CLASS(__ ## name) arg_list_p body) _PRAGMA(_)      \
     PROC_2(define MACRO_GLUE_2_(METHODS_CODE_PROTO_TOOL__, line)       \
            CLASS() CLASS(__ ## name) arg_list_p;) _PRAGMA(_))

PROC(define ___CALL_TOR2(line, name, arg_list, body...)     \
     ___CALL_TOR(line, name, arg_list, body))

//TODO: DTOR
