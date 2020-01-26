// transmit D
#include "tm4c123gh6pm.h"

void UART5Tx(char c);
void delayMS(int n);
void SystemInit(void);

const char transmit = 'd';

int main(void){
	SYSCTL->RCGCUART |= 0x20;
	SYSCTL->RCGCGPIO |= 0x10;
	
	UART5->CTL = 0;				/* disable UART0 */
	UART5->IBRD = 104;			/* 16MHz/16=1MHz, 1MHz/104=9600 baud rate */
	UART5->FBRD = 11;			/* fraction part, see Example 4-4 */
	UART5->CC = 0;				/* use system clock */
	UART5->LCRH = 0x60;			/* 8-bit, no parity, 1-stop bit, no FIFO */
	UART5->CTL = 0x301;			/* configure PA0, TXE, RXE */
	
	GPIOE->DEN = 0x30;
	GPIOE->AMSEL = 0;
	GPIOE->AFSEL = 0x30;
	GPIOE->PCTL = 0x00110000;
	
	delayMS(1);
	
	for(;;) {
		UART5Tx(transmit);
		delayMS(1000);
	}
}

void UART5Tx(char c) {
	while((UART5->FR & 0x20) != 0); /*wait til tx buffer not full to send data */
	UART5->DR = c;
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
