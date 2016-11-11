#include "inc.h"
#include "look.h"

char *look_for(buffer this, char *motif, char *before, action_type type)
{
    int motif_len = strlen(motif);
    char *copy = 0, *tmp = 0;
    bool in_str = false;
    char *skip_index = 0;

    ACTION
    while (!$.stream_finished)
    {
        if ($.index >= $.size)
        {
            action(0);
            __read(this);
            continue;
        }

        HANDLE_STR;
        
        tmp = strstr($.data + $.index, motif);
        if (tmp != NULL && (skip_index == NULL || tmp < skip_index)) {
            $.index = tmp - $.data;
            action(before);
            $.index = motif_len;
            discard(this);
            END_OK;
        }
        for (int i = $.size - motif_len + 1;
             i < $.size && i >= 0 && $.data + i < skip_index;
             i++) {
            if (strcmp($.data + i, motif) == 0)
            {
                $.index = i;
                action(0);
                __read(this);
                if (!$.stream_finished)
                    break;
                i--;
                continue;
            }
        }
        $.index = (skip_index ? skip_index - $.data: $.size);
    }
    END_KO;
}

char *balanced_look_for(buffer this, char motif, char cancel, char *before, action_type type)
{
    char *copy = 0, *tmp = 0;
    int depth = 0;
    bool started = false;

    ACTION
    while (!$.stream_finished || $.index < $.size)
    {
        if ($.index >= $.size)
        {
            action(0);
            __read(this);
            continue;
        }
        
        if (!started)
        {
            tmp = index($.data + $.index, motif);
            if (tmp != NULL) {
                $.index = tmp - $.data;
                started = true;
                action(before);
            }
            else
                $.index = $.size;
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
                    action(0);
                    END_OK;
                }
            }
        }
        $.index = $.size;
    }
    END_KO;
}
