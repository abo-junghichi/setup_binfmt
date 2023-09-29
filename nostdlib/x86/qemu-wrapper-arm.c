#include <asm/unistd.h>
static long syscall3(long syscall, long arg1, long arg2, long arg3)
{
    long ret;
    __asm__ volatile ("int $0x80":"=a" (ret):"a"(syscall), "b"(arg1),
		      "c"(arg2), "d"(arg3):"memory");
    return ret;
}
__attribute__((noreturn))
static int execve(const char *filename, char *const argv[],
		  char *const envp[])
{
    return syscall3(__NR_execve, filename, argv, envp);
}
static int main(int argc, char **argv, char **envp)
{
    unsigned int i, d = 2;
    char *newargv[argc + d];
    newargv[0] = argv[0];
    newargv[1] = "-B";
    newargv[2] = "0x10000";
    for (i = 1; i < argc; i++)
	newargv[i + d] = argv[i];
    newargv[argc + d] = (void *) 0;
    execve("/usr/bin/qemu-arm", newargv, envp);
}
void _start_main(char **ebp)
{
    int argc;
    char **argv, **envp;
    argc = ebp[0];
    argv = ebp + 1;
    envp = argv + argc + 1;
    main(argc, argv, envp);
}
asm(".globl _start\n" ".type _start, @function\n" "_start:\n" "push %esp\n"
    "call _start_main\n" ".size _start, .-_start\n");
