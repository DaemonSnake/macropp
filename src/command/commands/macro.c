/*
 * Copyright (C) 2016  Bastien Penavayre
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "macro++.h"
#include "tools/macro.c"

NEW_SUBCOMMAND(MACRO_OP, UNDEF)
{
    struct macro_node *it = find_macro(POP(hash));

    CLEAN();
    remove_macro(it);
    return true;
}

NEW_SUBCOMMAND(MACRO_OP, SET)
{
    size_t hash = POP(hash);
    char *opt RAII = POP();
    struct macro_node *it = find_macro(hash);

    CLEAN();
    update_macro(hash, it, opt, false);
    return true;
}

NEW_SUBCOMMAND(MACRO_OP, EVAL)
{
    size_t hash = POP(hash);
    char *opt RAII = POP();
    struct macro_node *it = find_macro(hash);

    CLEAN();
    update_macro(hash, it, opt, true);
    return true;
}

void expand_macro(buffer this, char *name)
{
    size_t hash = hash_string(name);
    struct macro_node *it = find_macro(hash);

    if (!it)
        return ;
    this->input_index += it->size;
    write(this->out, it->text, it->size);
}
