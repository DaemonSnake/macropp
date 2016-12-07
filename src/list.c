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
#include "inc.h"

struct list_node_item
{
    char *text;
    struct list_node_item *next;
    struct list_node_item *prev;
};

struct list_node
{
    size_t hash;
    size_t size;
    struct list_node_item *front;
    struct list_node_item *back;
    struct list_node *next;
    struct list_node *prev;
};

static struct
{
    struct list_node *front;
    struct list_node *back;
} list = {NULL, NULL};

static struct list_node *find_list_node(size_t hash)
{
    for (struct list_node *it = list.front; it != NULL; it = it->next)
        if (it->hash == hash)
            return it;
    return NULL;
}

void print_list_id(buffer this, size_t hash, char *sep)
{
    struct list_node *node = find_list_node(hash);
    struct list_node_item *it;

    if (node == NULL)
        return ;
    it = node->front;
    print_strs(this, it->text, NULL);
    for (it = it->next; it != NULL; it = it->next)
        print_strs(this, sep, it->text, NULL);
}

static struct list_node *create_node(size_t hash, char *first_text)
{
    struct list_node *ret = malloc(sizeof(struct list_node));
    struct list_node_item *first = malloc(sizeof(struct list_node_item));

    ret->hash = hash;
    ret->size = 1;
    ret->front = first;
    ret->back = first;
    ret->next = NULL;
    ret->prev = NULL;
    first->next = NULL;
    first->prev= NULL;
    first->text = first_text;
    return ret;
}

void list_push_front(size_t hash, char *front)
{
    struct list_node *node = find_list_node(hash);
    struct list_node_item *it = NULL;

    if (node == NULL) {
        node = create_node(hash, front);
        node->next = list.front;
        list.front = node;
        if (list.back == NULL)
            list.back = node;
        return ;
    }
    it = malloc(sizeof(struct list_node_item));
    it->text = front;
    it->next = node->front;
    it->prev = NULL;
    if (it->next)
        it->next->prev = it;
    node->front = it;
}

void list_push_back(size_t hash, char *back)
{
    struct list_node *node = find_list_node(hash);
    struct list_node_item *it = NULL;

    if (node == NULL) {
        node = create_node(hash, back);
        node->prev = list.back;
        list.back = node;
        if (list.front == NULL)
            list.front = node;
        return ;
    }
    it = malloc(sizeof(struct list_node_item));
    it->text = back;
    it->prev = node->back;
    it->next = NULL;
    if (it->prev)
        it->prev->next = it;
    node->back = it;
}

void list_pop_front(size_t hash)
{
    (void)hash;
}

void list_pop_back(size_t hash)
{
    (void)hash;
}

void list_remove_item(size_t hash, unsigned index)
{
    (void)hash;
    (void)index;
}

char *list_get_item(size_t hash, unsigned index)
{
    (void)hash;
    (void)index;
    return NULL;
}

__attribute__((destructor))
static void clear_all_lists()
{
    for (struct list_node *it = list.front, *next = NULL;
         it; ((next = it->next), free(it), (it = next)))
        for (struct list_node_item *item = it->front, *next = NULL;
             item; ((next = item->next), free(item), (item = next)))
            free(item->text);
    list.front = NULL;
    list.back = NULL;
}
