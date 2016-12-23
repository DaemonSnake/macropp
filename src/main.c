#define _GNU_SOURCE
#include "inc.h"
#include <mcheck.h>
#include <sys/stat.h>
#include <fcntl.h>

void loop(int in, int out)
{
    buffer buf = buffer_new(in, out);

    while (look_for(buf, IN_STR, NULL, true, PROCCESS))
        handle_arguments(buf);
    delete(buf);
}

int main(int ac, char **av)
{
    int in = 0, out = 1;
    bool in_open = false, out_open = false;

    /* mtrace(); */
    for (int i = 1; i < ac; )
    {
        if (!out_open && strcmp(av[i], "-o") == 0) {
            i++;
            if (out_open && out != -1)
                close(out);
            if ((out = open(av[i], O_TRUNC|O_CREAT|O_WRONLY, 0666)) == -1) {
                fprintf(stderr, "[Error BadOutFile] '%s' can't be created\n", av[i]);
                break;
            }
            out_open = true;
        }
        else if (!in_open) {
            if (in_open && in != -1)
                close(in);
            if ((in = open(av[i], O_RDONLY)) == -1) {
                fprintf(stderr, "[Error BadInFile] '%s' can't be opened\n", av[i]);
                break;
            }
            in_open = true;
        }
        i++;
    }
    if (in == -1 || out == -1) {
        if (in_open)
            close(in);
        if (out_open)
            close(out);
        return 42;
    }
    loop(in, out);
    return 0;
}
