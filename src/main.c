#include "uart.h"
#include "kprintf.h"
#include "trap.h"
#include "mm.h"

void kmain(unsigned long hartid, unsigned long dtb) {
    (void)hartid;
    (void)dtb;

    uart_init();
    kprintf("[Zentex OS] Kernel booting in S-Mode...\n");
    
    trap_init();
    kprintf("[Zentex OS] RISC-V 64-bit initialization successful.\n");

    mm_init();

    void *p1 = kalloc();
    void *p2 = kalloc();
    kprintf("[MM] Allocated pages at %p and %p\n", p1, p2);

    kprintf("[Zentex OS] Core initialization complete.\n");

    while (1) {
    __asm__ __volatile__("wfi");
    }
}