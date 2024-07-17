#include "uart.h"

#define UART_BASE 0x09000000

void uart_putc(char c) {
    volatile unsigned int *uartdr = (unsigned int *) UART_BASE;
    *uartdr = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

int puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
    uart_putc('\n');
    return 0;
}

int __io_putchar(int ch){
    uart_putc((char) ch);
    return ch;
}

