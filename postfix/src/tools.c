#include "inc.h"

buffer new(int in, int out)
{
    buffer this = malloc(sizeof(struct buffer));

    if (!this)
        return NULL;
    $.data = NULL;
    $.size = 0;
    $.index = 0;
    $.in = in;
    $.out = out;
    $.stream_finished = false;
    $.thread = NULL;
    return this;
}

buffer new_string(char *str, int out)
{
    buffer this = malloc(sizeof(struct buffer));

    if (!this)
        return NULL;
    $.data = str;
    $.size = strlen(str);
    $.index = 0;
    $.in = -1;
    $.out = out;
    $.stream_finished = false;
    $.thread = NULL;
    return this;
}

buffer new_transfer(buffer this, int new_in, int out)
{
    buffer tmp = new_string($.data, out);

    tmp->in = $.in;
    tmp->index = $.index;
    tmp->thread = $.thread;
    $.in = new_in;
    $.data = 0;
    $.size = 0;
    $.stream_finished = false;
    $.index = 0;
    $.thread = 0;
    return tmp;
}

void delete(buffer this)
{
    free($.data);
    free(this);
}

void exit_(buffer this)
{
    delete(this);
    exit(42);
}

void proccess(buffer this)
{
    if ($.size == 0)
        return ;
    write($.out, $.data, $.index);
    memmove($.data, $.data + $.index, $.size - $.index);
    $.size -= $.index;
    $.data[$.size] = '\0';
    $.index = 0;
}

void discard(buffer this)
{
    if ($.size == 0)
        return ;
    memmove($.data, $.data + $.index, $.size - $.index);
    $.size -= $.index;
    $.data[$.size] = '\0';
    $.index = 0;
}

void __read(buffer this)
{
    int len;

    if ($.stream_finished)
        return ;
    if ($.data == NULL)
        $.data = malloc(SIZE + 1);
    if ($.in == -1) {
        $.stream_finished = true;
        return ;
    }
    if ((len = read($.in, $.data + $.index, SIZE - $.index)) <= 0) {
        if ($.thread != NULL) {
            close($.in);
            pthread_join(*$.thread, (void **)&$.in);
            free($.thread);
            $.thread = NULL;
        }
        else {
            $.stream_finished = true;
            return ;
        }
    }
    $.size += len;
    $.data[$.size] = 0;
}
