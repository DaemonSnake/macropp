#include "inc.h"
#include "look.h"

char *look_for(buffer this, char *motif, char *before, bool swallow, action_type type)
{
    int motif_len = strlen(motif);
    char *result = 0, *found = 0;
    trilean in_literal = ab_false;

    while (!$.stream_finished)
    {
        NEW_READ;
        if (!(found = strstr($.data + $.index, motif))) {
            $.index = $.size - motif_len;
            ACTION(0);
            __read(this);
            continue ;
        }
        if (!ignore_literals(this, &in_literal, found))
            continue;

        $.index = found - $.data;
        ACTION(before);
        $.index += motif_len;
        (swallow ? discard(this) : ACTION(0));
        END_OK;
    }
    END_KO;
}

char *balanced_look_for(buffer this, char motif, char cancel, char *before,
                        bool swallow, action_type type)
{
    char *result = 0, *in = 0, *out = 0;
    int depth = 0;
    bool started = false;
    trilean in_literal = false;

    while (!$.stream_finished)
    {
        NEW_READ;

        if (!started)
        {
            if (!(in = index($.data + $.index, motif))) {
                $.index = $.size;
                continue ;
            }
            if (!ignore_literals(this, &in_literal, in))
                continue ;

            $.index = in - $.data;
            started = true;
            ACTION(before);
            if (swallow) {
                $.index++;
                discard(this);
                depth = 1;
            }
        }

        out = min_str(index($.data + $.index, motif),
                      index($.data + $.index, cancel));
        if (!out) {
            $.index = $.size;
            continue ;
        }

        if (!ignore_literals(this, &in_literal, out))
            continue ;

        $.index = out - $.data + 1;
        if (*out == cancel && --depth == 0)
        {
            $.index = out - $.data + (swallow ? 0 : 1);
            ACTION(0);
            if (swallow) {
                $.index++;
                discard(this);
            }
            END_OK;
        }
        else if (*out == motif)
            depth++;
    }
    END_KO;
}

static char *index_inf(char *str, char motif, char *other)
{
    if (!(str = index(str, motif)) || str > other)
        return NULL;
    return str;
}

static void action(buffer this, action_type type, char **result, char *before)
{
    if ($.size == 0)
        return ;
    $.index = ($.index < 0 ? 0 :
               ($.index > $.size ? $.size : $.index));
    if (type == COPY) {
        *result = append_string(append_string_n(*result, $.data, $.index),
                             before);
        discard(this);
    }
    else if (type == PROCCESS) {
        proccess_found(this, (before != NULL), before);
        *result = (void*)0x42;
    }
}

static bool ignore_literals(buffer this, trilean *in_literal, char *found)
{
    char *index_str = index_inf($.data + $.index, '"', found),
        *index_char = index_inf($.data + $.index, '\'', found);

    if (*in_literal) {
        char symb = (*in_literal == a_true ? '"' : '\''),
            *to_finish = (symb == '"' ? index_str : index_char),
            *out = index_without_escape(to_finish + 1, symb);

        if (out == NULL) {
            $.index = $.size;
            return false;
        }
        $.index = out - $.data + 1;
        if (out > found)
            return false;
        *in_literal = ab_false;
    }

    for (; index_str || index_char;
         index_str = index_inf($.data + $.index, '"', found),
         index_char = index_inf($.data + $.index, '\'', found))
    {
        char *min = min_str(index_str, index_char),
            symb = (min == index_str ? '"' : '\''),
            *max = (symb == '"' ? index_char : index_str),
            *out = index_without_escape(min + 1, symb);

        if (out == NULL) {
            *in_literal = (symb == '"' ? a_true : b_true);
            $.index = $.size;
            return false;
        }
        $.index = out - $.data + 1;

        if (out > found)
            return false;
        if (!max || max < out)
            continue ;

        if (!(out = index(max + 1, symb))) {
            $.index = $.size;
            return false;
        }
        $.index = out - $.data;
        if (out > found)
            return false;
    }
    return true;
}
