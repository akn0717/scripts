#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define SETLINES(stream) while ((len = getdelim(&line, &size, delim, stream)) != -1) { \
                             tmpstr = malloc((len - 1) * sizeof(char)); \
                             memcpy(tmpstr, line, len - 1); \
                             lines[i++] = tmpstr; \
                             if (i == n) \
                                 lines = realloc(line, (n *= 2) * sizeof(char *)); \
                         }

int main(int argc, char *argv[])
{
    int n = 100, nfile, i, j, k;
    int *ihist, *iarr;
    unsigned int lprint = 0, nprint = 0, nsame = 1, nsbuf = 2;
    char delim = '\n';
    char *line = NULL, *tmpstr = NULL;
    char **lines = malloc(n * sizeof(char *)), **files;
    size_t size;
    ssize_t len;
    FILE *file;

    while ((i = getopt(argc, argv, "l:n:z0")) != -1) {
        switch (i) {
            case 'l':
                errno = 0;
                lprint = strtoul(optarg, &tmpstr, 10);
                if (errno) {
                    perror("strtoul");
                    exit(EXIT_FAILURE);
                }
                if (optarg == tmpstr) {
                    fputs("ERROR[shufr]: Invalid number given to option -l.\n", stderr);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n':
                errno = 0;
                nsbuf = 2 * (nsame = strtoul(optarg, &tmpstr, 10));
                if (errno) {
                    perror("strtoul");
                    exit(EXIT_FAILURE);
                }
                if (optarg == tmpstr) {
                    fputs("ERROR[shufr]: Invalid number given to option -n.\n", stderr);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'z':
            case '0':
                delim = '\0';
                break;
        }
    }

    files = argv + optind;
    nfile = argc - optind;

    i = 0;
    if (nfile == 0) {
        SETLINES(stdin);
    } else for (j = 0; j < nfile; j++) {
        file = fopen(files[j], "r");
        SETLINES(file);
        fclose(file);
    }

    lines = realloc(lines, (n = i) * sizeof(char *));
    ihist = malloc(nsbuf * sizeof(int));
    iarr  = malloc(nsame * sizeof(int));

    for (i = 0; i < nsbuf; i++) {
        ihist[i] = -1;
    }

    srand(time(NULL));
    for (;;) {
newcycle:
        for (i = 0; i < nsame; i++) {
            k = rand() % n;
            for (j = 0; j < nsbuf; ++j) {
                if (ihist[j] == k)
                    goto newcycle;
            }
            ihist[nsame + i] = iarr[i] = k;
        }
        for (i = 0; i < nsame; i++) {
            printf("%s%c", lines[ihist[i] = iarr[i]], delim);
            if (lprint && ++nprint == lprint)
                exit(EXIT_SUCCESS);
        }
        for (i = nsame; i < nsbuf; i++) {
            ihist[i] = -1;
        }
    }

    return 0;
}
