#include "tm4c123gh6pm.h"

char UART0Rx(void);
void delayMS(int n);
void SystemInit(void);

int main(void) {
	char c;
	
	SYSCTL->RCGCUART |= 1;
	SYSCTL->RCGCGPIO |= 1;
	SYSCTL->RCGCGPIO |= 0x20;
	
	UART0->CTL = 0;				/* disable UART0 */
	UART0->IBRD = 104;			/* 16MHz/16=1MHz, 1MHz/104=9600 baud rate */
	UART0->FBRD = 11;			/* fraction part, see Example 4-4 */
	UART0->CC = 0;				/* use system clock */
	UART0->LCRH = 0x60;			/* 8-bit, no parity, 1-stop bit, no FIFO */
	UART0->CTL = 0x301;			/* configure PA0, TXE, RXE */
	
	GPIOA->DEN = 0x03;
	GPIOA->AFSEL = 0x03;
	GPIOA->PCTL = 0x11;
	
	GPIOF->DIR = 0x0E;
	GPIOF->DEN = 0x0E;
	GPIOF->DATA = 0;
	
	for(;;) {
		c = UART0Rx();
		GPIOF->DATA = c << 1;
	}
}

/* Recieves and returns chars transmitted to the board via UART. */
char UART0Rx(void) {
	char c;
	while((UART0->FR & 0x10) != 0); /* Waits for buffer to clear */
	c = UART0->DR; /* read data */
	return c;
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