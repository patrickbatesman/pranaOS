/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cpu.h>
#include <kernel/console.h>

/**
 * @brief enable sse
 * 
 */
extern "C" void enableSSE();

/**
 * @brief cpuid
 * 
 * @param reg 
 * @param eax 
 * @param ebx 
 * @param ecx 
 * @param edx 
 */
static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}
