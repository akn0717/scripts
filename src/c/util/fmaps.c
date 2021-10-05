#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../include/strutil.h"

#define DIE(ERRMSG) { fputs("fmaps: "ERRMSG"\n", stderr); \
                      exit(EXIT_FAILURE); }

int main(int argc, char *argv[])
{
    char *sep, *end, *def, *line, *eol, *s;
    int i, c, offset;
    size_t len;
    ssize_t nread;

    end = getenv("END");
    if (!end)
        end = "\n";

    if (argc == 1) {
        while((c = getchar()) != EOF) {
            if (c != '\n')
                putchar(c);
            else
                fputs(end, stdout);
        }
        return 0;
    }

    sep = getenv("SEP");
    if (!sep)
        sep = "=";
    offset = strlen(sep);

    for (i = 1; i < argc; i++) {
        s = strstr(argv[i], sep);
        if (!s)
            DIE("argument does not contain separator");
        *s = '\0';
    }

    def = getenv("DEF");

    line = NULL;
    len = 0;
    while ((nread = getline(&line, &len, stdin)) != -1) {
        eol = line;
        while(*++eol);
        eol = eol - 1;
        if (*eol == '\n')
            *eol = '\0';
        for (i = 1; i < argc; i++) {
            if (streq(line, argv[i])) {
                printf("%s%s", argv[i] + strlen(argv[i]) + offset, end);
                goto newline;
            }
        }
        if (!def)
            printf("%s%s", line, end);
        else
            printf("%s%s", def, end);
newline: ;
    }

    return 0;
}
