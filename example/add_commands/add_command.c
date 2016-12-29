#include "inc.h"

struct function create_function(char *name, char *code)
{
    int fd[2];
    struct function result = {0, name, NULL, NULL};
    pid_t pid;
    char *command = NULL;

    asprintf(&result.libname, "/tmp/lib_tmp_%s.so", name);
    asprintf(&command, str(gcc -shared -x c /dev/stdin -fPIC -o %s), result.libname);
    pipe(fd);
    if ((pid = fork()) == 0)
    {
        close(fd[1]);
        close(1);
        dup2(fd[0], 0);
        execvp("sh", (char * const[]){ "sh", "-c", command, NULL});
        free(command);
        close(fd[0]);
        fprintf(stderr, "[ERROR]Coudn't produce custom command\n");
        exit(42);
    }
    else
    {
        free(command);
        result.pid = pid;
        close(fd[0]);
        write(fd[1], "#include <stdio.h>\n", 19);
        write(fd[1], "void entry()\n{", 14);
        write(fd[1], code, strlen(code));
        write(fd[1], "}", 1);
        close(fd[1]);
    }
    return result;
}

void call_function(struct function *func)
{
    if (func->func == NULL)
    {
        int status = 0;
        waitpid(func->pid, &status, 0);
        if (status)
            return ;
        func->handle = dlopen(func->libname, RTLD_LAZY);
        func->func = dlsym(func->handle, "entry");
    }
    func->func();
}

void delete_function(struct function *func)
{
    free(func->libname);
    free(func->name);
    dlclose(func->handle);
    bzero(func, sizeof(struct function));
}

int i = 0;
