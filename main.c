#include <stdio.h>
#include "String.c"

int main()
{
    String ref(tmp) = new(String, C)("Thing");

    printf("value: %s\n", $m(tmp, c_str));
    printf("%p\n", tmp->_vtable->getType);
    return 0;
}
