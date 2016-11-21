#include "inc.h"

int main()
{
    buffer buf = new(0, 1);

    while (look_for(buf, "[@", NULL, true, PROCCESS)) {
        //struct array results = get_argument_list(buf);
        handle_arguments(buf, look_for(buf, " @]", NULL, true, COPY));
    }
    delete(buf);
}
