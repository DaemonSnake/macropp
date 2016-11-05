#include "inc.h"

buffer new(int in, int out)
{
    buffer this = malloc(sizeof(struct buffer));

    if (this)
    {
        $.data = 0;
        $.size = 0;
        $.index = 0;
        $.in = in;
        $.out = out;
        $.stream_finished = false;
    }
    return this;
}

buffer new_string(char *str, int out)
{
    buffer this = malloc(sizeof(struct buffer));

    if (this)
    {
        $.data = str;
        $.size = strlen(str);
        $.index = 0;
        $.in = -1;
        $.out = out;
        $.stream_finished = false;
    }
    return this;
}

void delete(buffer this)
{
    free($.data);
    free(this);
}

void exit_(buffer this)
{
    delete(this);
    exit(1);
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
    if ($.in == -1 || (len = read($.in, $.data + $.index, SIZE - $.index)) <= 0) {
        $.stream_finished = true;
        return ;
    }
    $.size += len;
    $.data[$.size] = 0;
}
