#include "inc.h"

int main()
{
    buffer buf = new(0, 1);

    while (look_for(buf, "[@", NULL, true, PROCCESS)) {
        handle_arguments(buf, look_for(buf, " @]", NULL, true, COPY));
    }
    delete(buf);
}
