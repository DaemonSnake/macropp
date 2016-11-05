#include "inc.h"

void rec_postfix(buffer buf)
{
    char *argument;
    char *body;
    
    while (look_for(buf, "__POSTFIX__(@", PROCCESS))
    {
        if ((argument = look_for(buf, "@)", COPY)))
        {
            if ((body = balanced_look_for(buf, '{', '}', COPY)))
            {
                buffer b = new_string(body, 1);
                rec_postfix(b);
                b->index = b->size;
                proccess(b);
                delete(b);
            }
            printf(" %s", argument);
            fflush(stdout);
            free(argument);
        }
    }
}

int main()
{
    buffer buf = new(0, 1);

    rec_postfix(buf);
    delete(buf);
}
