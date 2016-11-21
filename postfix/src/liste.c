#include <string.h>

struct macro_item
{
    char *text;
    size_t size;
    
    struct macro_item *next;
    struct macro_item *prev;
};

struct macro
{
    char *name;

    struct macro_item *front;
    struct macro_item *back;

    struct macro *next;
    struct macro *prev;
};

struct
{
    struct macro *front;
    struct macro *back;
} instance = {NULL, NULL};

#define NEW(name) macro_list_ ## name __ADD_NAME__
#define __ADD_NAME__(args...) (char *name, ##args)
#define LIST struct macro *list = find_macro(name)

static struct macro *find_macro(char *name)
{
    for (struct macro *it = instance.front; it != NULL; it = it->next)
        if (strcmp(name, it->name) == 0)
            return it;
    return NULL;
}

void NEW(push_front)(char *text)
{
    LIST;
}

void NEW(push_back)(char *text)
{
    LIST;
}

char *NEW(pop_front)()
{
    LIST;
}

char *NEW(pop_back)()
{
    LIST;
}

void NEW(iterate)(char *template)
{
}

void NEW(clear)()
{
    LIST;
}

void NEW(print)(int fd)
{
    LIST;
}
