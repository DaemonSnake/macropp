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
#include "inc.h"

typedef struct
{
    int pipe[2];
    buffer buf;
    bool (*function)(buffer, struct array);
    struct array args;
} data;

static void *thread_reader2(data *this)
{
    $.function($.buf, $.args);
    $.buf->index = $.buf->size;
    proccess($.buf);
    close($.pipe[1]);
    free(this);
    return NULL;
}

void spawn_command(buffer buf, bool(*function)(buffer, struct array), struct array args)
{
    data *argument = malloc(sizeof(data));
    pthread_t thread;

    pipe(argument->pipe);
    argument->function = function;
    argument->args = args;
    argument->buf = buffer_new_transfer(buf, argument->pipe[0], argument->pipe[1]);
    pthread_create(&thread, 0, (void* (*)(void*))thread_reader2, argument);
    pthread_detach(thread);
}
