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
    $.next = NULL;
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
    $.next = NULL;
    return this;
}

buffer new_transfer(buffer this, int new_in, int out)
{
    buffer tmp = new_string($.data, out);

    tmp->in = $.in;
    tmp->index = $.index;
    tmp->next = $.next;
    $.in = new_in;
    $.data = 0;
    $.size = 0;
    $.stream_finished = false;
    $.index = 0;
    $.next = tmp;
    return tmp;
}

void transfer_back(buffer this, buffer other)
{
    $.next = other->next;
    close($.in);
    $.in = other->in;
    delete(other);
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
    if ($.index >= $.size)
    {
        write($.out, $.data, $.size);
        free($.data);
        $.data = NULL;
        $.index = 0;
        $.size = 0;
        return ;
    }
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
    if ($.index >= $.size)
    {
        free($.data);
        $.data = NULL;
        $.index = 0;
        $.size = 0;
        return ;
    }
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
    if ($.data == NULL) {
        $.data = malloc(SIZE + 1);
        bzero($.data, SIZE + 1);
    }
    if ($.in == -1) {
        $.stream_finished = true;
        return ;
    }

    while ((len = read($.in, $.data + $.size, SIZE - $.size)) <= 0)
        if ($.next != NULL)
            transfer_back(this, $.next);
        else
            return (void)($.stream_finished = true);
    $.size += len;
    $.data[$.size] = '\0';
}
