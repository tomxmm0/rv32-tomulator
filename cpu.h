#pragma once

#include <stdint.h>
#include "bus.h"

typedef struct CPU {
    uint32_t registers[32];
    uint32_t pc;
    uint32_t next_pc;

    BUS bus;
} CPU;

void cpu_init(CPU* cpu);
uint32_t cpu_fetch(CPU* cpu);
void cpu_execute(CPU* cpu, uint32_t instruction);
