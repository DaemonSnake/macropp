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
    char *before, *after;
    union {
        struct {
            char *motif;
        };
        struct {
            char in, out;
        };
    };
    enum { LOOK, BALENCED } type;
} data;

static int thread_reader(data *this)
{
    if ($.type == LOOK) {
        look_for($.buf, $.motif, $.before, PROCCESS);
        free($.motif);
    }
    else
        balanced_look_for($.buf, $.in, $.out, $.before, PROCCESS);

    write($.buf->out, $.after, strlen($.after));
    FREE($.before, $.after);
    //NOT GOOD -> linked list of buffer instead and fixed pthread_t per buffer
    while (!$.buf->stream_finished)
    {
        $.buf->index = $.buf->size;
        proccess($.buf);
        if ($.buf->thread)
            __read($.buf);
        else
            break;
    }
    int tmp = $.buf->in;
    delete($.buf);
    close($.pipe[1]);
    free(this);
    return tmp;
}

void spawn_look(buffer buf, char *motif, char *before, char *after)
{
    data *argument = malloc(sizeof(data));
    pthread_t *thread = malloc(sizeof(pthread_t));

    pipe(argument->pipe);
    argument->type =  LOOK;
    argument->motif = motif;
    argument->before = before;
    argument->after = after;
    argument->type = LOOK;
    argument->buf = new_transfer(buf, argument->pipe[0], argument->pipe[1]);
    buf->thread = thread;
    pthread_create(buf->thread, 0, (void* (*)(void*))thread_reader, argument);
}

void spawn_balenced_look(buffer buf, char in, char out, char *before, char *after)
{
    data *argument = malloc(sizeof(data));
    pthread_t *thread = malloc(sizeof(pthread_t));

    pipe(argument->pipe);
    argument->type =  BALENCED;
    argument->in = in;
    argument->out = out;
    argument->before = before;
    argument->after = after;
    argument->type = BALENCED;
    argument->buf = new_transfer(buf, argument->pipe[0], argument->pipe[1]);
    buf->thread = thread;
    pthread_create(buf->thread, 0, (void* (*)(void*))thread_reader, argument);
}
