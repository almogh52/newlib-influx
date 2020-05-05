#ifndef _SYS_DIRENT_H
#define _SYS_DIRENT_H

#include <bits/dirent.h>
#include <sys/types.h>

#define _LIBC 1
#define NOT_IN_libc 1
#include <sys/lock.h>
#undef _LIBC

#define HAVE_NO_D_NAMLEN /* no struct dirent->d_namlen */
#define HAVE_DD_LOCK     /* have locking mechanism */

typedef struct {
    int dd_fd;  /* directory file */
    int dd_loc; /* position in buffer */
    int dd_seek;
    char *dd_buf; /* buffer */
    int dd_len;   /* buffer length */
    int dd_size;  /* amount of data in buffer */
    _LOCK_RECURSIVE_T dd_lock;
} DIR;

#define __dirfd(dir) (dir)->dd_fd

#endif