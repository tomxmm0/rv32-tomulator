#pragma once

#include "dram.h"

typedef struct BUS {
    DRAM dram;
} BUS;

uint32_t bus_load(BUS* bus, uint32_t address, size_t size);
void bus_store(BUS* bus, uint32_t address, size_t size, uint32_t value);
