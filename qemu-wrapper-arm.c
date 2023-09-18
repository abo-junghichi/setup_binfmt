#include <unistd.h>
int main(int argc, char **argv, char **envp)
{
    size_t i, d = 2;
    char *newargv[argc + d];
    newargv[0] = argv[0];
    newargv[1] = "-B";
    newargv[2] = "0x10000";
    for (i = 1; i < argc; i++)
	newargv[i + d] = argv[i];
    newargv[argc + d] = NULL;
    return execve("/usr/bin/qemu-arm", newargv, envp);
}
