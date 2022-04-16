//
//  memory.h
//  pranaOS
//
//  Created by Krisna Pranav on 13/01/22.
//

#pragma once

#include <ak/types.h>
#include <ak/convert.h>
#include <ak/memoperator.h>
#include <kernel/console.h>
#include <multiboot/multiboot.h>

namespace Kernel {
    #define BLOCK_SIZE 4_KB
    #define BLOCKS_PER_BYTE 8

    typedef struct multibootMemoryMap {
        unsigned int size;
        unsigned long base_addr_low;
        unsigned long base_addr_high;
        unsigned long length_low;
        unsigned long length_high;
        unsigned int type;
    }  __attribute__((packed)) grub_multiboot_memory_map_t;

    class physicalMemoryManager {
    public:
        static void initialize(uint32_t size, uint32_t bitmap);
        static void setRegionFree(uint32_t base, uint32_t size);
        static void setRegionUsed(uint32_t base, uint32_t size);
        static void parseMemoryMap(const multiboot_info_t* mbi);
            
        static void* allocateBlock();
        static void freeBlock(void* ptr);
        static void* allocateBlocks(uint32_t size);
        static void freeBlocks(void* ptr, uint32_t size);

        static uint32_t amountOfMemory();
        static uint32_t usedBlocks();
        static uint32_t freeBlocks();
        static uint32_t totalBlocks();
        static uint32_t getBitmapSize();
            
    private:
        static uint32_t memorySize;
        static uint32_t usedBlocks;
        static uint32_t maximumBlocks;
        static uint32_t* memoryArray;

        static inline void setBit (uint32_t bit) {
            memoryArray[bit / 32] |= (1 << (bit % 32));
        }

        static inline void unsetBit (uint32_t bit) {
            memoryArray[bit / 32] &= ~ (1 << (bit % 32));
        }

        static inline bool testBit (uint32_t bit) {
            return memoryArray[bit / 32] &  (1 << (bit % 32));
        }

        static uint32_t FirstFree ();
        static uint32_t FirstFreeSize (uint32_t size);
    };

    uint32_t pageRoundUp(uint32_t address);
    uint32_t pageRoundDown(uint32_t address);
}
