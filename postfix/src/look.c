#include "inc.h"
#include "look.h"

char *look_for(buffer this, char *motif, char *before, bool swallow, action_type type)
{
    int motif_len = strlen(motif);
    char *copy = 0, *found = 0;
    bool in_str = false, in_char = false;

    ACTION;
    while (!$.stream_finished)
    {
        NEW_READ;
        found = strstr($.data + $.index, motif);
        HANDLE_STR(in_str, '"', found);
        HANDLE_STR(in_char, '\'', found);
        if (found != NULL) {
            $.index = found - $.data;
            action(before);
            $.index += motif_len;
            (swallow ? discard(this) : action(0));
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
    char *copy = 0, *in = 0, *out = 0;
    int depth = 0;
    bool started = false, in_str = false, in_char = false;

    ACTION
    while (!$.stream_finished)
    {
        NEW_READ;
        
        if (!started)
        {
            in = index($.data + $.index, motif);
            HANDLE_STR(in_str, '"', in);
            HANDLE_STR(in_char, '\'', in);
            if (in)
            {
                $.index = in - $.data;
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

        out = min_str(index_without_escape($.data + $.index, motif),
                      index_without_escape($.data + $.index, cancel));
        HANDLE_STR(in_str, '"', out);
        HANDLE_STR(in_char, '\'', out);

        if (!out)
        {
            $.index = $.size;
            continue;
        }
        $.index = out - $.data + 1;
        if (*out == cancel && --depth == 0)
        {
            $.index = out - $.data + (swallow ? 0 : 1);
            action(0);
            if (swallow) {
                $.index = out - $.data;
                discard(this);
            }
            END_OK;
        }
        else if (*out == motif)
            depth++;
    }
    END_KO;
}
