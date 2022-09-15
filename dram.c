#include <stdio.h>
#include "dram.h"

#define DRAM_DBG(__FMT__, ...) printf("[DRAM]: "__FMT__ "\n", __VA_ARGS__)
#define DRAM_FAULT(__FMT__, ...) printf("[DRAM FAULT]: "__FMT__"\n", __VA_ARGS__); \
    getchar()


uint32_t dram_load(DRAM* dram, uint32_t address, size_t size)
{
    DRAM_DBG("Loading %d bytes from address %p", size, address);

    switch (size)
    {
        case 1:
            return *(uint8_t*)&dram->memory[address-DRAM_BASE];
        case 2:
            return *(uint16_t*)&dram->memory[address-DRAM_BASE];
        case 4:
            return *(uint32_t*)&dram->memory[address-DRAM_BASE];
        default:
            DRAM_FAULT("Invalid load size %d", size);
            return 0;
    }
}

void dram_store(DRAM* dram, uint32_t address, size_t size, uint32_t value)
{
    DRAM_DBG("Storing value %d with size %d at address %p", value, size, address);

    switch (size)
    {
        case 1:
            *(uint8_t*)&dram->memory[address-DRAM_BASE] = value;
            break;
        case 2:
            *(uint16_t*)&dram->memory[address-DRAM_BASE] = value;
            break;
        case 4:
            *(uint32_t*)&dram->memory[address-DRAM_BASE] = value;
            break;
        default:
            DRAM_FAULT("Invalid store size %d", size);
    }
}
