#include <stdio.h>
#include <errno.h>

#include "emulator.h"

int main(int argc, const char** argv)
{
    if (argc != 2)
    {
        puts("Usage: \"./emulator [file name]\"\n");
        return 1;
    }

    CPU cpu;

    if (emulator_load_binary(&cpu, argv[1]))
    {
        printf("Error loading file %s: %d\n", argv[1], errno);
        return 1;
    }

    emulator_start(&cpu);
    return 0;
}
