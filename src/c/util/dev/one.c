#include <unistd.h>

int main(int argc, char *argv[])
{
    const unsigned short data[] = { 0xff };
    for (;; write(1, data, 1)) {}
}
