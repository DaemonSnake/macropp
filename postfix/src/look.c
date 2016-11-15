#include "inc.h"
#include "look.h"

char *look_for(buffer this, char *motif, char *before, action_type type)
{
    int motif_len = strlen(motif);
    char *copy = 0, *found = 0;
    bool in_str = false;

    ACTION;
    while (!$.stream_finished)
    {
        NEW_READ;
        found = strstr($.data + $.index, motif);
        HANDLE_STR;
        if (found != NULL) {
            $.index = found - $.data;
            action(before);
            $.index = motif_len;
            discard(this);
            END_OK;
        }
        $.index = $.size - motif_len;
        action(0);
        __read(this);
    }
    END_KO;
}

char *balanced_look_for(buffer this, char motif, char cancel, char *before,
                        bool swallow, action_type type)
{
    char *copy = 0, *found = 0;
    int depth = 0;
    bool started = false;

    ACTION
    while (!$.stream_finished)
    {
        NEW_READ;
        
        if (!started)
        {
            if ((found = index($.data + $.index, motif))) {
                $.index = found - $.data;
                started = true;
                action(before);
                if (swallow) {
                    $.index++;
                    discard(this);
                    depth = 1;
                }
            }
            else
                $.index = $.size;
            continue;
        }
        for (int i = $.index; i < $.size; i++)
            if ($.data[i] == motif)
                depth++;
            else if ($.data[i] == cancel)
                if (--depth == 0)
                {
                    $.index = i + (swallow ? 0 : 1);
                    action(0);
                    if (swallow) {
                        $.index = i;
                        discard(this);
                    }
                    END_OK;
                }
        $.index = $.size;
    }
    END_KO;
}
