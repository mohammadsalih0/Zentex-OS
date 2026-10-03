#include "kprintf.h"
#include "uart.h"
#include <stdarg.h>
#include <stdint.h>

static void print_num(uint64_t num, int base, int is_signed) {
    char buf[65];
    char *digits = "0123456789abcdef";
    int i = 0;

    if (is_signed && (int64_t)num < 0) {
        uart_putc('-');
        num = (uint64_t)(-(int64_t)num);
    }

    if (num == 0) {
        uart_putc('0');
        return;
    }

    while (num > 0) {
        buf[i++] = digits[num % base];
        num /= base;
    }

    while (--i >= 0) {
        uart_putc(buf[i]);
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

for (const char *p = fmt; *p != '\0'; p++) {
    if (*p != '%') {
        uart_putc(*p);
        continue;
    }

    p++; // Skip '%'
    switch (*p)
    {
    case 'c':
        uart_putc((char)va_arg(args, int));
        break;
    case 's': {
        char *s = va_arg(args, char*);
        uart_puts(s ? s : "(null)");
        break;
    }

    case 'd':
        print_num(va_arg(args, int64_t), 10, 1);
        break;
    case 'u':
        print_num(va_arg(args, uint64_t), 10, 0);
        break;
    case 'x':
    case 'p':
        uart_puts("0x");
        print_num(va_arg(args, uint64_t), 16, 0);
        break;
    case '%':
        uart_putc('%');
        break;
    default:
        uart_putc('%');
        uart_putc(*p);
        break;
    }
}
va_end(args);
}