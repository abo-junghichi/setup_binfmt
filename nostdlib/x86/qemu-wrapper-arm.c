#include <asm/unistd.h>
#define FAST_SYSCALL
#ifdef FAST_SYSCALL
#include <elf.h>
#include <stddef.h>
#endif
static long syscall3(long syscall, long arg1, long arg2, long arg3)
{
    long ret;
#ifdef FAST_SYSCALL
    void *__kernel_vsyscall = NULL;
#define Syscall_core "call *%1"
#define Syscall_entry "g"(__kernel_vsyscall)
/*
The Definitive Guide to Linux System Calls | Packagecloud Blog
https://blog.packagecloud.io/the-definitive-guide-to-linux-system-calls/
The Linux kernel: System Calls
https://www.win.tue.nl/~aeb/linux/lk/lk-4.html
*/
    {
	Elf32_auxv_t *auxv;
	char **wenvp = arg3;
	while (*wenvp++ != NULL);
	for (auxv = (Elf32_auxv_t *) wenvp; AT_NULL != auxv->a_type;
	     auxv++)
	    if (AT_SYSINFO == auxv->a_type) {
		__kernel_vsyscall = auxv->a_un.a_val;
		break;
	    }
    }
#else
#define Syscall_core "int $0x80"
#define Syscall_entry "i"(0)
#endif
    __asm__ volatile (Syscall_core:"=a"(ret):Syscall_entry, "a"(syscall),
		      "b"(arg1), "c"(arg2), "d"(arg3):"memory");
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
