#define _GNU_SOURCE
#include "inc.h"
#include <mcheck.h>

void loop()
{
    buffer buf = buffer_new(0, 1);

    while (look_for(buf, "[@", NULL, true, PROCCESS))
        handle_arguments(buf);
    delete(buf);
}

int main()
{
    /* mtrace(); */
    loop();
    return 0;
}
