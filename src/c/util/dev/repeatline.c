#include <stdio.h>

int main(int argc, char *argv[])
{
    int i, l = argc - 1;

    if (argc < 2)
        return 0;

    for (;;) {
        for (i = 1; i < l; i++) {
            fputs(argv[i], stdout);
            putchar(' ');
        }
        puts(argv[l]);
    }
}
