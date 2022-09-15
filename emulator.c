#include <stdio.h>
#include "emulator.h"

const char* register_to_abi(int x_num)
{
    switch (x_num)
    {
        case 0:
            return "zero";
        case 1:
            return "ra";
        case 2:
            return "sp";
        case 3:
            return "gp";
        case 4:
            return "tp";
        case 5:
            return "t0";
        case 6:
            return "t1";
        case 7:
            return "t2";
        case 8:
            return "fp";
        case 9:
            return "s1";
        case 10:
            return "a0";
        case 11:
            return "a1";
        case 12:
            return "a2";
        case 13:
            return "a3";
        case 14:
            return "a4";
        case 15:
            return "a5";
        case 16:
            return "a6";
        case 17:
            return "a7";
        case 18:
            return "s2";
        case 19:
            return "s3";
        case 20:
            return "s4";
        case 21:
            return "s5";
        case 22:
            return "s6";
        case 23:
            return "s7";
        case 24:
            return "s8";
        case 25:
            return "s9";
        case 26:
            return "s10";
        case 27:
            return "s11";
        case 28:
            return "t3";
        case 29:
            return "t4";
        case 30:
            return "t5";
        case 31:
            return "t6";
        default:
            return "unk";
    }
}

void emulator_dump_registers(CPU* cpu)
{
    for (int i = 0; i < 32; i++)
    {
        printf("%s: %x, ", register_to_abi(i), cpu->registers[i]);
    }

    putchar('\n');
}

int emulator_load_binary(CPU* cpu, const char* filename)
{
    FILE* bin = fopen(filename, "rb");
    if (bin == NULL)
    {
        return 1;
    }

    fseek(bin, 0, SEEK_END);
    size_t size = ftell(bin);
    fseek(bin, 0, SEEK_SET);

    if (fread(&cpu->bus.dram.memory, size, 1, bin) != 1)
    {
        fclose(bin);
        return 1;
    }

    fclose(bin);
    return 0;
}

void emulator_start(CPU* cpu)
{
    cpu_init(cpu);

    while (1) 
    {
        uint32_t inst = cpu_fetch(cpu);

        emulator_dump_registers(cpu);
        cpu_execute(cpu, inst);

        if (cpu->pc == 0)
        {
            return;
        }
    }
}
