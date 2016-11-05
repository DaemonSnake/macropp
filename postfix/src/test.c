#include "inc.h"

char *look_for(buffer this, char *motif, action_type type)
{
    int motif_len = strlen(motif);
    char *copy = 0, *tmp = 0;

    void action()
    {
        if (type == COPY) {
            char *tmp = 0;
            asprintf(&tmp, "%s%.*s", (copy == 0 ? "" : copy), $.index, $.data);
            free(copy);
            copy = tmp;
            discard(this);
        }
        else if (type == PROCCESS) {
            proccess(this);
            copy = (void*)0x42;
        }
    }

    while (!$.stream_finished)
    {
        if ($.index >= $.size)
        {
            action();
            __read(this);
            continue;
        }
        tmp = strstr($.data + $.index, motif);
        if (tmp != NULL) {
            fflush(stdout);
            $.index = tmp - $.data;
            action();
            $.index = motif_len;
            discard(this);
            return copy;
        }
        for (int i = $.size - motif_len + 1; i < $.size && i >= 0; i++) {
            if (strcmp($.data + i, motif) == 0)
            {
                $.index = i;
                action();
                __read(this);
                if (!$.stream_finished)
                    break;
                continue;
            }
        }
        $.index = $.size;
    }
    return copy;
}

char *balanced_look_for(buffer this, char motif, char cancel, action_type type)
{
    char *copy = 0, *tmp = 0;
    int depth = 0;
    bool started = false;

    void action()
    {
        if (type == COPY) {
            char *tmp = 0;
            asprintf(&tmp, "%s%.*s", (copy == 0 ? "" : copy), $.index, $.data);
            free(copy);
            copy = tmp;
            discard(this);
        }
        else if (type == PROCCESS) {
            proccess(this);
            copy = (void*)0x42;
        }
    }

    while (!$.stream_finished)
    {
        if ($.index >= $.size)
        {
            action();
            __read(this);
            continue;
        }
        if (!started)
        {
            tmp = index($.data + $.index, motif);
            if (tmp == NULL)
                $.index = $.size;
            else {
                $.index = tmp - $.data - 1;
                proccess(this);
                started = true;
            }
            continue;
        }
        for (int i = $.index; i < $.size; i++)
        {
            if ($.data[i] == motif)
                depth++;
            else if ($.data[i] == cancel)
            {
                if (--depth == 0)
                {
                    $.index = i + 1;
                    action();
                    return copy;
                }
            }
        }
        $.index = $.size;
    }
    return copy;
}
