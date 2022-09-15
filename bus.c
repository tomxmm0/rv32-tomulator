#include <stdio.h>
#include "dram.h"
#include "bus.h"

#define BUS_DBG(__FMT__, ...) printf("[BUS]: "__FMT__ "\n", __VA_ARGS__)
#define BUS_FAULT(__FMT__, ...) printf("[BUS FAULT]: "__FMT__"\n", __VA_ARGS__); \
    getchar()

uint32_t bus_load(BUS* bus, uint32_t address, size_t size)
{
    return dram_load(&bus->dram, address, size);
}

void bus_store(BUS* bus, uint32_t address, size_t size, uint32_t value)
{
    dram_store(&bus->dram, address, size, value);
}
