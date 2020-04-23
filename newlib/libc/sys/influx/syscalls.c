#include <syscall.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <stdarg.h>
#include <stdio.h>

_syscall1_base(void, _exit, 0, int, status)
_syscall1_base(int, close, 1, int, file)
_syscall3_base(int, execve, 2, char *, name, char **, argv, char **, env)
_syscall0_base(int, fork, 3)
_syscall2_base(int, fstat, 4, int, file, struct stat *, st)
_syscall0_base(int, getpid, 5)
_syscall1_base(int, isatty, 6, int, file)
_syscall2_base(int, kill, 7, int, pid, int, sig)
_syscall2_base(int, link, 8, char *, old, char *, new)
_syscall3_base(int, lseek, 9, int, file, int, ptr, int, dir)
_syscall3_base(int, r_open, 10, const char *, name, int, flags, int, mode)
_syscall3_base(int, read, 11, int, file, char *, ptr, int, len)
_syscall1_base(caddr_t, sbrk, 12, int, incr)
_syscall2_base(int, stat, 13, const char *,file, struct stat *, st)
_syscall1_base(clock_t, times, 14, struct tms *, buf)
_syscall1_base(int, unlink, 15, char *, name)
_syscall1_base(int, wait, 16, int *, wstatus)
_syscall3_base(int, write, 17, int, file, char *, ptr, int, len)
_syscall2_base(_sig_func_ptr, signal, 18, int, signum, _sig_func_ptr, handler)
_syscall2_base(int, gettimeofday, 19, struct timeval *, p, void *, z)

int open (const char *name, int flags, ...) {
	va_list list;

	va_start(list, flags);

	int ret = r_open(name, flags, va_arg(list, int));

	va_end(list);

	return ret;
}