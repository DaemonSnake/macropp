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

#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define $m(arg, func, args...) arg->funcs->func(arg, ##args)
#define $this(func, args...) this->funcs->func(this, ##args)
#define $ (*this)
#define SIZE 262144
#define FREE(front, args...) free_all(front, ##args, 0)
#define RAII __attribute__((cleanup(free_ptr)))
#define IN_STR "[@"
#define OUT_STR "@]"
#define SEP_STR "@,"
#define IN_STR_SIZE 2
#define OUT_STR_SIZE 2
#define SEP_STR_SIZE 2
#define POP(opt...)                                                     \
    MANGLE_OPT(pop_front_argument, ##opt)(_Generic(args, struct array: &args, struct array *: args))
#define CLEAN() free_arguments(_Generic(args, struct array: &args, struct array *: args))
#define NEW_HANDLE(name) bool handle_ ## name(buffer buf, struct array args)
#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])

typedef enum { false = 0, true } bool;
typedef enum { PROCCESS, COPY } action_type;

typedef struct buffer *buffer;
struct buffer
{
    char *data;
    int size;
    int read_index;
    int in, out;
    bool stream_finished;
    buffer next;
    size_t input_index;
    const struct buffer_vtable *funcs;
};

struct buffer_vtable
{
    void (*delete)(buffer this);
    void (*process)(buffer this);
    void (*proccess_found)(buffer this, bool finished, char *after);
    void (*discard)(buffer this);
    void (*read)(buffer this);
};

struct array
{
    char **data;
    char **begin;
    unsigned size;
};

//BUFFER
buffer buffer_new(int in, int out);
buffer buffer_new_string(char *str, int out);
buffer buffer_new_transfer(buffer this, int new_in, int out);
void print_size(buffer this, size_t i);
void print_strs(buffer this, ...);

char *look_for(buffer this, char *motif, char *before, char *after, bool swallow, action_type type);
char *balanced_look_for(buffer this, char motif, char cancel, char *before, char *after, bool swallow, action_type type);

//ARGS | ARRAY
void handle_arguments(buffer buf);
char *get_argument(char *arg_list, int index);
bool fill_argument_list(buffer this, struct array *res);
char *pop_front_argument(struct array *arg);
void free_arguments(struct array *);
bool fill_argument_list_from_string(char *arg_list, struct array *res);
size_t pop_front_argument_hash(struct array *arg);
unsigned pop_front_argument_to_uint(struct array *arg);
int pop_front_argument_to_int(struct array *arg);
double pop_front_argument_to_double(struct array *arg);

//TOOLS
void free_all(void *, ...);
void raii_free(void *);
void free_ptr(void *);

//THREADS
char *eval_string_command(char *str);
void spawn_command(buffer buf, bool(*function)(buffer, struct array), struct array);

//MACRO
void expand_macro(buffer, char *);
void macro_handling(struct array);

//LIST
void list_push_back(size_t, char *);
void list_push_front(size_t, char *);
void print_list_id(buffer, size_t, char *);
void list_pop_front(size_t hash);
void list_pop_back(size_t hash);
void list_remove_item(size_t hash, unsigned index);
char *list_get_item(size_t hash, unsigned index);
void list_parse_parenth(size_t hash, char *value);
void list_clear(size_t hash);
void list_eval(size_t hash, int index);

//STRING TOOLS
char *min_str(char *, char *);
int string_index(char *motif, ...);
int int_index(char *str, char car);
char *replace_special_characters(char *str);
char *index_without_escape(char *str, char c);
char *append_string(char *or, char *new_end);
char *append_string_n(char *or, char *new_end, int size_end);
size_t hash_string(char *);
bool is_identifier(char *str);
void unchar_string(char *str);
bool number_in_string(double number, char *str, size_t size);
char *skip_seperator_end(char *begin, char *end);
char *skip_seperator(char *begin);
char *get_end_of_argument(char *list, bool rec, char **end, char *motifs[3]);

//MACRO_TOOLS
#define MANGLE(x, y) __MANGLE_1(x, y)
#define CMANGLE(x) MANGLE(COMMAND, x)
#define __MANGLE_1(x, y) __MANGLE_2(x, y)
#define __MANGLE_2(x, y) x ## __ ## y
#define NEW_SUBCOMMAND(name, sub)                                       \
    bool MANGLE(name, sub ## __ ## subcommand)(buffer this __attribute__((unused)), struct array args)

#define __MANGLE_DO(x, y) x ## _ ## y
#define __MANGLE_DONT(x, y) x
#define __MANGLE_OPT(value, opt, macro, tool...) macro(value, opt)
#define MANGLE_OPT(value, opt...) __MANGLE_OPT(value, ##opt, __MANGLE_DO, __MANGLE_DONT)
