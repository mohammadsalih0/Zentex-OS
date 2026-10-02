#include "uart.h"

#define UART0_BASE  0x10000000L
#define RBR_THR     0x00
#define LSR         0x05
#define LSR_THRE    0x20

static volatile uint8_t * const UART0 = (uint8_t *)UART0_BASE;

void uart_init(void) {
    /* QEMU virt UART is auto-configured; no baud rate clock setup needed */
}

void uart_putc(char c) {
    while ((UART0[LSR] & LSR_THRE) == 0);
    UART0[RBR_THR] = c;
}

void uart_puts(const char *s) {
    while (*s) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s++);
    }
}