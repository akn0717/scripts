#ifndef HEADER_SCRIPTS_SYSUTIL
#define HEADER_SCRIPTS_SYSUTIL

#ifdef _POSIX_C_SOURCE
#define BAK_POSIX_C_SOURCE _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#undef _POSIX_C_SOURCE
#else /* ifdef _POSIX_C_SOURCE */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#endif /* ifndef _POSIX_C_SOURCE */

#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <strutil.h>

#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif /* ifdef _POSIX_C_SOURCE */

#ifdef BAK_POSIX_C_SOURCE
#define _POSIX_C_SOURCE BAK_POSIX_C_SOURCE
#undef BAK_POSIX_C_SOURCE
#endif /* ifdef BAK_POSIX_C_SOURCE */

/* path is modified in the process, but restored to original */
void rmkparent(char *path, mode_t mode)
{
    size_t i, j, size = strlen(path) + 1;

    for (i = 1, j = 0; i < size; i++) {
        if (path[i] == '/') {
            if (j)
                path[j] = '/';
            path[j = i] = '\0';
            mkdir(path, mode);
        }
    }
    if (j)
        path[j] = '/';
}

void rmkparentconst(const char *path, mode_t mode)
{
    char *pathdup = astrdup(path);
    rmkparent(pathdup, mode);
    free(pathdup);
}

/* path is modified in the process, but restored to original */
void rmkfile(char *path, mode_t mode)
{
    int fd;

    rmkparent(path, mode);
    if ((fd = open(path, O_CREAT, mode)) != -1)
        close(fd);
}

void rmkfileconst(const char *path, mode_t mode)
{
    char *pathdup = astrdup(path);
    rmkfile(pathdup, mode);
    free(pathdup);
}

/* path is modified in the process, but restored to original */
void rmkdir(char *path, mode_t mode)
{
    rmkparent(path, mode);
    mkdir(path, mode);
}

void rmkdirconst(const char *path, mode_t mode)
{
    char *pathdup = astrdup(path);
    rmkdir(pathdup, mode);
    free(pathdup);
}

/* no restrict intentional */
char *simpslashnbuf(const char *path, char *buf, size_t bufsize)
{
    char c;
    size_t i = 0;
    bool isslash = false;

    while ((c = *path)) {
        if (c == '/') {
            if (isslash) {
                path++;
                continue;
            }
            buf[i] = c;
            isslash = true;
        } else {
            buf[i] = c;
            isslash = false;
        }
        path++;
        i++;
    }
    if (buf[i - 1] == '/')
        buf[i - 1] = '\0';
    else
        buf[i] = '\0';

    return buf;
}

/* no restrict intentional */
char *simpslashbuf(const char *path, char *buf)
{
    const char *bufbeg = buf;
    char c;
    bool isslash = false;

    while ((c = *path)) {
        if (c == '/') {
            if (isslash) {
                path++;
                continue;
            }
            *buf = c;
            isslash = true;
        } else {
            *buf = c;
            isslash = false;
        }
        path++;
        buf++;
    }
    if (buf[-1] == '/')
        buf[-1] = '\0';
    else
        *buf = '\0';

    return (char *)bufbeg;
}

char *simpslash(char *path)
{
    simpslashbuf(path, path);
    return path;
}

#endif /* ifndef HEADER_SCRIPTS_SYSUTIL */
