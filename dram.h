#pragma once

#include <stdint.h>
#include <stddef.h>

#define DRAM_SIZE 1024*1024*1 // 1 MiB
#define DRAM_BASE 0x80000000 

typedef struct DRAM {
    uint8_t memory[DRAM_SIZE];
} DRAM;


uint32_t dram_load(DRAM* dram, uint32_t address, size_t size);
void dram_store(DRAM* dram, uint32_t address, size_t size, uint32_t value);
