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

#define $ (*this)
#define SIZE 2048
#define FREE(front, args...) free_all(front, ##args, 0)
#define RAII __attribute__((cleanup(free_ptr)))
#define IN_STR "[@"
#define OUT_STR "@]"
#define SEP_STR "@,"
#define IN_STR_SIZE 2
#define OUT_STR_SIZE 2
#define SEP_STR_SIZE 2
#define GET(i) pop_argument(&args, i + 1)
#define CLEAN() free_arguments(&args)
#define NEW_HANDLE(name) bool handle_ ## name(buffer buf, struct array args)

typedef enum { false = 0, true } bool;
typedef enum { PROCCESS, COPY } action_type;

typedef struct buffer *buffer;
struct buffer
{
    char *data;
    int size;
    int index;
    int in, out;
    bool stream_finished;
    buffer next;
    size_t input_index;
};

struct array
{
    char **data;
    unsigned size;
};

buffer buffer_new(int in, int out);
buffer buffer_new_string(char *str, int out);
buffer buffer_new_transfer(buffer this, int new_in, int out);
void delete(buffer this);
void exit_(buffer this);
void proccess(buffer this);
void proccess_found(buffer this, bool finished, char *after);
void discard(buffer this);
void __read(buffer this);
char *look_for(buffer this, char *motif, char *before, bool swallow, action_type type);
char *balanced_look_for(buffer this, char motif, char cancel,
                        char *before, bool swallow, action_type type);
void rec_postfix(buffer buf);
void update_index(buffer this, int index);
void handle_arguments(buffer buf);
char *get_argument(char *arg_list, int index);
void free_all(void *, ...);
void raii_free(void *);
int string_index(char *motif, ...);
int int_index(char *str, char car);
char *replace_special_characters(char *str);
void print_size(buffer this, size_t i);
void print_strs(buffer this, ...);
void spawn_command(buffer buf, bool(*function)(buffer, struct array), struct array);
void free_ptr(void *);
char *index_without_escape(char *str, char c);
char *min_str(char *, char *);
char *append_string(char *or, char *new_end);
char *append_string_n(char *or, char *new_end, int size_end);
bool fill_argument_list(buffer this, struct array *res);
void free_arguments(struct array *);
char *pop_argument(struct array *arg, unsigned index);
size_t hash_string(char *);
void expand_macro(buffer, char *);
void macro_handling(struct array);
bool is_identifier(char *str);
void unchar_string(char *str);
bool fill_argument_list_from_string(char *arg_list, struct array *res);
void list_push_back(size_t, char *);
void list_push_front(size_t, char *);
void print_list_id(buffer, size_t, char *);
void list_pop_front(size_t hash);
void list_pop_back(size_t hash);
void list_remove_item(size_t hash, unsigned index);
char *list_get_item(size_t hash, unsigned index);
void list_parse_parenth(size_t hash, char *value);
void list_clear(size_t hash);
char *eval_string_command(char *str);
char *skip_seperator(char *str);
bool number_in_string(double number, char *str, size_t size);
char *skip_seperator_end(char *begin, char *end);
char *skip_seperator(char *begin);
