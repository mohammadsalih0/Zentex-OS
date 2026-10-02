#include "uart.h"

void kmain(unsigned long hartid, unsigned long dtb) {
    (void)hartid;
    (void)dtb;

    uart_init();
    uart_puts("[Zentex OS] Kernel booting in S-Mode...\n");
    uart_puts("[Zentex OS] RISC-V 64-bit initialization successful.\n");

    while (1) {
    __asm__ __volatile__("wfi");
    }
}