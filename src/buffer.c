#include "macro++.h"
#include <stdarg.h>

static void proccess(buffer this);
static void proccess_found(buffer this, bool finished, char *after);
static void discard(buffer this);
static void buffer_read(buffer this);
static void delete(buffer this);

static const struct buffer_vtable vtable_instance = {
    delete,
    proccess,
    proccess_found,
    discard,
    buffer_read
};

buffer buffer_new(int in, int out)
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
    $.input_index = 0;
    $.funcs = &vtable_instance;
    return this;
}

buffer buffer_new_string(char *str, int out)
{
    buffer this = malloc(sizeof(struct buffer));

    if (!this)
        return NULL;
    $.data = str;
    $.size = (str == NULL ? 0 : strlen(str));
    $.index = 0;
    $.in = -1;
    $.out = out;
    $.stream_finished = false;
    $.next = NULL;
    $.input_index = 0;
    $.funcs = &vtable_instance;
    return this;
}

buffer buffer_new_transfer(buffer this, int new_in, int out)
{
    buffer tmp = buffer_new_string($.data, out);

    if (!this)
        return NULL;
    tmp->in = $.in;
    tmp->index = $.index;
    tmp->next = $.next;
    tmp->input_index = $.input_index;
    $.in = new_in;
    $.data = 0;
    $.size = 0;
    $.stream_finished = false;
    $.index = 0;
    $.next = tmp;
    return tmp;
}

static void transfer_back(buffer this, buffer other)
{
    $.next = other->next;
    close($.in);
    $.in = other->in;
    delete(other);
}

static void delete(buffer this)
{
    free($.data);
    free(this);
}

static void proccess(buffer this)
{
    proccess_found(this, false, NULL);
}

static void proccess_found(buffer this, bool finished, char *after)
{
    if ($.size == 0)
        return ;
    if ($.index >= $.size)
    {
        $.input_index += $.size;
        if ($.out != -1)
            write($.out, $.data, $.size);
        *$.data = '\0';
        $.index = 0;
        $.size = 0;
        if (finished) {
            print_strs(this, after, 0);
            //alt
        }
        return ;
    }
    $.input_index += $.index;
    if ($.out != -1)
        write($.out, $.data, $.index);
    memmove($.data, $.data + $.index, $.size - $.index);
    $.size -= $.index;
    $.data[$.size] = '\0';
    $.index = 0;
    if (finished) {
        print_strs(this, after, 0);
        //alt
    }
}

static void discard(buffer this)
{
    if ($.size == 0)
        return ;
    if ($.index >= $.size)
    {
        *$.data = '\0';
        $.index = 0;
        $.size = 0;
        return ;
    }
    memmove($.data, $.data + $.index, $.size - $.index);
    $.size -= $.index;
    $.data[$.size] = '\0';
    $.index = 0;
}

static void buffer_read(buffer this)
{
    int len;

    if ($.stream_finished)
        return ;
    if ($.data == NULL) {
        $.data = malloc(SIZE + 1);
        *$.data = '\0';
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

void print_strs(buffer this, ...)
{
    va_list ap;
    char *str;
    int len;

    va_start(ap, this);
    while ((str = va_arg(ap, char *)) != NULL) {
        len = strlen(str);
        this->input_index += len;
        if ($.out != -1)
            write(this->out, str, len);
    }
    va_end(ap);
}

void print_size(buffer this, size_t i)
{
    char *number;
    int len;

    asprintf(&number, "%lu", i);
    len = strlen(number);
    this->input_index += len;
    if ($.out != -1)
        write(this->out, number, len);
    free(number);
}
