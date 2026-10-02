#ifndef RISCV_H
#define RISCV_H

#include <stdint.h>

static inline uint64_t r_sstatus(void) {
    uint64_t x;
    __asm__ __volatile__("csrr %0, sstatus" : "=r" (x));
    return x;
}

static inline void w_sstatus(uint64_t x) {
    __asm__ __volatile__("csrw sstatus, %0" : : "r" (x));
}

static inline uint64_t r_tp(void) {
    uint64_t x;
    __asm__ __volatile__("mv %0, tp" : "=r" (x));
    return x;
}

#endif