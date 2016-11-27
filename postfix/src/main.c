#include "inc.h"
#include <mcheck.h>
#include <fcntl.h>
#include <sys/stat.h>

__attribute__((constructor))
static void mem()
{
    mtrace();
}

int main()
{
    buffer buf = new(0, 1);

    while (look_for(buf, "[@", NULL, true, PROCCESS))
        handle_arguments(buf);
    delete(buf);
    return 0;
}
