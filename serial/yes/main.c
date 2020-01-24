#include "tm4c123gh6pm.h"

void UART0Tx(char c);
void delayMS(int n);
void SystemInit(void);

int main(void) {
	SYSCTL->RCGCUART |= 1;
	SYSCTL->RCGCGPIO |= 1;
	
	/* UART 0 INIT */
	UART0->CTL = 0; 			/* disable UART0 */
	UART0->IBRD = 104;		/* ?? 16MHz/16 = 1 MHz, 1MHz/104=9600 baud rate */
	UART0->FBRD = 11;			/* ?? fraction part, see example 4-4 in textbook */
	UART0->CC = 0;				/* use system clock */
	UART0->LCRH = 0x60;		/* 8 bit, no parity, 1-stop bit, no FIFO */
	UART0->CTL = 0x301;		/* enable UART0, TXE, RXE */
	
	/* UART0 TX0 and RX0 use PA0 and PA1. Set them up. */
	GPIOA->DEN = 0x03;
	GPIOA->AFSEL = 0x03;	/* Use PA0, PA1 alternate function */
	GPIOA->PCTL = 0x11;		/* configure PA0, and PA1 for UART */
	
	delayMS(1);
	
	for(;;) {
		UART0Tx('Y');
		UART0Tx('E');
		UART0Tx('S');
		UART0Tx(' ');
	}
}

void UART0Tx(char c) {
	while((UART0->FR & 0x20) != 0); /*wait til tx buffer not full to send data */
	UART0->DR = c;
}

void delayMS(int n) {
	int i, j;
	for(i=0; i < n; i++){
		for(j=0; j < 3180; j++) {}
	}
}

void SystemInit(void) {
	SCB->CPACR |= 0x00F00000;
}