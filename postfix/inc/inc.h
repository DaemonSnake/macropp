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
};

buffer new(int in, int out);
buffer new_string(char *str, int out);
void delete(buffer this);
void exit_(buffer this);
void proccess(buffer this);
void discard(buffer this);
void __read(buffer this);
char *look_for(buffer this, char *motif, char *before, action_type type);
char *balanced_look_for(buffer this, char motif, char cancel,
                        char *before, bool swallow, action_type type);
void rec_postfix(buffer buf);
int append_string(char **str, char *buf, int size);
void update_index(buffer this, int index);
int skip_separator(char *string);
void handle_arguments(buffer buf, char *arguments);
char *get_argument(char *arg_list, int index);
void free_all(void *, ...);
void raii_free(void *);
buffer new_transfer(buffer this, int new_in, int out);
int string_index(char *motif, ...);
int int_index(char *str, char car);
char *replace_special_characters(char *str);
void print_size(int fd, size_t i);
void print_strs(int fd, ...);
void spawn_command(buffer buf, void(*function)(buffer, char *), char *arg, int offset);
void free_ptr(void *);
char *index_without_escape(char *str, char c);
char *min_str(char *, char *);
