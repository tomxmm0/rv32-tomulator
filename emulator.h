#pragma once

#include "cpu.h"

int emulator_load_binary(CPU* cpu, const char* filename);
void emulator_start(CPU* cpu);
