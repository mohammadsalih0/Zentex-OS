#include "trap.h"
#include "kprintf.h"

extern void trap_vector(void);

void trap_init(void) {
    // Set stvec to point to trap_vector (Direct mode, bit 0 = 0)
    uintptr_t addr = (uintptr_t)trap_vector & ~3UL;
    __asm__ __volatile__("csrw stvec, %0" : : "r"(addr));
}

void trap_handler(struct trap_frame *tf) {
    uint64_t is_interrupt = (tf->scause >> 63) & 1;
    uint64_t cause = tf->scause & 0x7FFFFFFFFFFFFFFF;

    if (is_interrupt) {
        kprintf("[INTERRUPT] Code: %d\n", cause);
    } else {
        kprintf("\n[KERNEL PANIC] Unhandled Exception\n");
        kprintf("   scause: %x\n", tf->scause);
        kprintf("   sepc: %x\n", tf->sepc);
        kprintf("   stval: %x\n", tf->stval);
        while (1) { __asm__ __volatile__("wfi"); }
    }
}