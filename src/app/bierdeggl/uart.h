#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>
#include <stdio.h>
void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

void uart_init(void);

extern FILE uart_output;
extern FILE uart_input;
//FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
//FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

#endif
