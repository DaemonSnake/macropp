#include "macro++.h"
#include "tools/look.h"

bool look_for(buffer this, char *motif, char *before, char *after,
              bool swallow, action_type type, char **copy)
{
    int motif_len = strlen(motif);
    char *found = 0;
    trilean in_literal = ab_false;

    while (!$.stream_finished)
    {
        NEW_READ;
        found = strstr($.data + $.read_index, motif);
        if (!ignore_literals(this, &in_literal, found))
            continue;
        if (!found) {
            $.read_index = $.size - motif_len;
            ACTION(0);
            $this(read);
            continue ;
        }
        $.read_index = found - $.data;
        ACTION(before);
        $.read_index += motif_len;
        (swallow ? $this(discard) : ACTION(0));
        END_OK;
    }
    END_KO;
}

bool balanced_look_for(buffer this, char motif, char cancel, char *before, char *after,
                       bool swallow, action_type type, char **copy)
{
    char *in = 0, *out = 0;
    int depth = 0;
    bool started = false;
    trilean in_literal = false;

    while (!$.stream_finished)
    {
        NEW_READ;

        if (!started)
        {
            in = index($.data + $.read_index, motif);
            if (!ignore_literals(this, &in_literal, in))
                continue ;
            if (!(in)) {
                $.read_index = $.size;
                continue ;
            }
            $.read_index = in - $.data;
            started = true;
            ACTION(before);
            if (swallow) {
                $.read_index++;
                $this(discard);
                depth = 1;
            }
        }

        out = min_str(index($.data + $.read_index, motif),
                      index($.data + $.read_index, cancel));
        if (!ignore_literals(this, &in_literal, out))
            continue ;
        if (!out) {
            $.read_index = $.size;
            continue ;
        }

        $.read_index = out - $.data + 1;
        if (*out == cancel && --depth == 0)
        {
            $.read_index = out - $.data + (swallow ? 0 : 1);
            ACTION(0);
            if (swallow) {
                $.read_index++;
                $this(discard);
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
    if (!(str = index(str, motif)) || (other && str > other))
        return NULL;
    return str;
}

static void action(buffer this, action_type type, char **result, char *before)
{
    if ($.size == 0)
        return ;
    $.read_index = ($.read_index < 0 ? 0 :
               ($.read_index > $.size ? $.size : $.read_index));
    if (type == COPY) {
        *result = append_string(append_string_n(*result, $.data, $.read_index),
                             before);
        $this(discard);
    }
    else if (type == PROCCESS)
        $this(proccess_found, (before != NULL), before);
}

static bool ignore_literals(buffer this, trilean *in_literal, char *found)
{
    char *index_str = index_inf($.data + $.read_index, '"', found),
        *index_char = index_inf($.data + $.read_index, '\'', found);

    if (*in_literal) {
        bool is_str = (*in_literal == a_true);
        char *to_finish = (is_str ? index_str : index_char),
            *out = (to_finish && *to_finish ?
                    index_without_escape(to_finish + 1, (is_str ? '"' : '\'')) :
                    NULL);

        if (out == NULL) {
            $.read_index = $.size;
            return false;
        }
        $.read_index = out - $.data + 1;
        if (out > found)
            return false;
        index_str = (index_str <= out ? NULL: index_str);
        index_char = (index_char <= out ? NULL : index_char);
        *in_literal = ab_false;
    }

    for (; index_str || index_char;
         index_str = index_inf($.data + $.read_index, '"', found),
         index_char = index_inf($.data + $.read_index, '\'', found))
    {
        char *min = min_str(index_str, index_char),
            symb = (min == index_str ? '"' : '\''),
            *max = (symb == '"' ? index_char : index_str),
            *out = index_without_escape(min + 1, symb);

        if (out == NULL) {
            *in_literal = (symb == '"' ? a_true : b_true);
            $.read_index = $.size;
            return false;
        }
        $.read_index = out - $.data + 1;

        if (out > found)
            return false;
        if (!max || max < out)
            continue ;

        if (!(out = index(max + 1, symb))) {
            $.read_index = $.size;
            return false;
        }
        $.read_index = out - $.data;
        if (out > found)
            return false;
    }
    return true;
}
